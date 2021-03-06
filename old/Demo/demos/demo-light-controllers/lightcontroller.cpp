#include "lightcontroller.h"
#include "lightcontrollerpwmchannel.h"
#include "lightcontrollerrgbchannel.h"
#include "lightcontrollervoltagechannel.h"

//#include <QRandomGenerator>
//#include <QTimer>

#if defined (Q_OS_MAC)
#include <QBluetoothUuid>
#else
#include <QBluetoothAddress>
#endif

namespace il {

namespace  {
const QBluetoothUuid uuidService(QStringLiteral("6E400001-B5A3-F393-E0A9-E50E24DCCA9E"));
const QBluetoothUuid writeCharacteristicUuid(QStringLiteral("6E400002-B5A3-F393-E0A9-E50E24DCCA9E"));
const QBluetoothUuid readCharacteristicUuid(QStringLiteral("6E400003-B5A3-F393-E0A9-E50E24DCCA9E"));

//void setRandomValue(int msec, LightControllerVoltageChannel* channel)
//{
//    QTimer::singleShot(msec, [=](){
//        auto rand = QRandomGenerator::global();
//        double value = double(rand->generate())
//                / rand->max()
//                * (channel->maxValue() - channel->minValue())
//                + (channel->minValue());
//        qDebug() << "new value from timer:" << value;
//        channel->set_value(int(value));
//    });
//}
}

LightController::LightController(const QBluetoothDeviceInfo &info, QObject *parent)
    : AbstractLightController (parent)
    , m_info { info }
{
    update_name(m_info.name());
    update_address(address(m_info));
}

LightController::~LightController()
{
    clear();
}

void LightController::clear()
{
    AbstractLightController::clear();

    m_hasReceivedInitialState = false;

    m_notificationDescriptor = QLowEnergyDescriptor();

    m_service.reset();

    if (!m_controller.isNull()) {
        m_controller->disconnectFromDevice();
        m_controller.reset();
    }

    update_isBusy(false);

    qDebug() << "light controller internal state cleared";
}

QString LightController::address(const QBluetoothDeviceInfo &info)
{
#if defined(Q_OS_MAC)
    // workaround for Core Bluetooth:
    return info.deviceUuid().toString();
#else
    return info.address().toString();
#endif
}

bool LightController::isValidDevice(const QBluetoothDeviceInfo &info)
{
    return info.serviceUuids().contains(uuidService);
}

void LightController::connectToController()
{
    // just for safety
    clear();

    update_isBusy(true);

    m_controller.reset(new QLowEnergyController(m_info, this));

    connect(m_controller.get(), &QLowEnergyController::serviceDiscovered,
            this, &LightController::serviceDiscovered);
    connect(m_controller.get(), &QLowEnergyController::discoveryFinished,
            this, &LightController::serviceScanDone);
    connect(m_controller.get(), qOverload<QLowEnergyController::Error>(&QLowEnergyController::error),
            this, &LightController::controllerError);
    connect(m_controller.get(), &QLowEnergyController::connected,
            this, &LightController::controllerConnected);
    connect(m_controller.get(), &QLowEnergyController::disconnected,
            this, &LightController::controllerDisconnected);

    m_controller->connectToDevice();
}

void LightController::disconnectFromController()
{
    //disable notifications before disconnecting
    if (m_notificationDescriptor.isValid()
            && m_service
            && m_notificationDescriptor.value() == QByteArray::fromHex("0100")) {
        m_service->writeDescriptor(m_notificationDescriptor, QByteArray::fromHex("0000"));
    } else {
        clear();
    }
}

void LightController::serviceDiscovered(const QBluetoothUuid & uuid)
{
    if (uuid == uuidService) {
        if (m_service.isNull()) {
            update_message("Service found");
            qDebug() << "service found";
            m_service.reset(m_controller->createServiceObject(uuid, this));
        } else {
            qWarning() << "service already found";
        }
    }
}

void LightController::serviceScanDone()
{
    qDebug() <<  "service scan done";

    if (!m_service.isNull()) {
        connect(m_service.get(), &QLowEnergyService::stateChanged,
                this, &LightController::serviceStateChanged);
        connect(m_service.get(), &QLowEnergyService::characteristicChanged,
                this, &LightController::serviceCharacteristicChanged);
        connect(m_service.get(), &QLowEnergyService::descriptorWritten,
                this, &LightController::serviceDescriptorWritten);
        connect(m_service.get(), qOverload<QLowEnergyService::ServiceError>(&QLowEnergyService::error),
                this, &LightController::serviceError);
        m_service->discoverDetails();
        update_message("Connecting to service");
        qDebug() << "connecting to service...";
    } else {
        update_message("No service found");
        update_isBusy(false);
        qDebug() << "no service found";
    }
}

void LightController::controllerError(QLowEnergyController::Error error)
{
    update_message("Cannot connect to controller");
    qWarning() << "Controller Error:" << error;
}

void LightController::controllerConnected()
{
    qDebug() << "connected to controller; discovering services...";
    m_controller->discoverServices();
}

void LightController::controllerDisconnected()
{
    update_message("Controller disconnected");
    qWarning() << "controller disconnected";
}

void LightController::serviceStateChanged(QLowEnergyService::ServiceState state)
{
    qDebug() << "service state changed" << state;

    switch (state) {
    case QLowEnergyService::ServiceDiscovered: {
        const QLowEnergyCharacteristic readCharacteristic = m_service->characteristic(readCharacteristicUuid);
        if (!readCharacteristic.isValid()) {
            update_message("Cannot get read characteristic");
            qDebug() << "cannot get read characteristic";
            break;
        }

        m_notificationDescriptor = readCharacteristic.descriptor(QBluetoothUuid::ClientCharacteristicConfiguration);
        if (!m_notificationDescriptor.isValid()) {
            update_message("Cannot set notification descriptor");
            qWarning() << "cannot set notification descriptor";
            break;
        }

        m_service->writeDescriptor(m_notificationDescriptor, QByteArray::fromHex("0100"));
        update_message("Connected");
        qDebug() << "connected";

        writeToDevice("U?\n");

        break;
    }
    default:
        //nothing for now
        break;
    }
}

void LightController::serviceCharacteristicChanged(const QLowEnergyCharacteristic &characteristic, const QByteArray &data)
{
    // ignore any other characteristic change -> shouldn't really happen though
    if (characteristic.uuid() != QBluetoothUuid(readCharacteristicUuid))
        return;

    qDebug() << "received response: " << data;

    updateFromDevice(data.simplified());
}

void LightController::serviceDescriptorWritten(const QLowEnergyDescriptor &descriptor, const QByteArray &data)
{
    if (descriptor.isValid()
            && descriptor == m_notificationDescriptor
            && data == QByteArray::fromHex("0000")) {
        // disabled notifications -> assume disconnect intent
        clear();
    }
}

void LightController::serviceError(QLowEnergyService::ServiceError e)
{
    switch (e) {
    case QLowEnergyService::DescriptorWriteError:
        update_message("Cannot obtain service notifications");
        qWarning() << "cannot obtain service notifications";
        break;
    default:
        qWarning() << "service error:" << e;
    }
}

void LightController::updateFromDevice(const QByteArray &data)
{
    if (data.length() != 11) {
        update_message("Received invalid data");
        qWarning() << "received invalid data received from controller: 11 !=" << data.length() << "data:" << data;
    } else {
        if(data.startsWith("*")) {
            if(m_command.startsWith("U?")) {
                m_hasReceivedInitialState = true;
                clearChannels();

                auto controllerType = data.right(2).toInt();
                switch (controllerType) {
                case 2:
                    // 2 x Analogic
                    update_controllerType(V1_2x10V);
                    for (int i = 0; i < 2; ++i) {
                        auto channel = new LightControllerVoltageChannel(QString::number(i+1), this);
                        get_voltageChannels()->append(channel);
                        channel->set_value(data.mid(1 + i*2, 2).toInt(nullptr, 16));
                        connect(channel, &LightControllerVoltageChannel::valueChanged, this, &LightController::updateDevice);
                        // for testing, to make sure we don't break signal-slot connections from ui
                        // setRandomValue(3000, channel);
                        // setRandomValue(6000, channel);
                    }
                    break;
                case 3:
                    // 4 x PWM
                    update_controllerType(V1_4xPWM);
                    for (int i = 0; i < 4; ++i) {
                        auto channel = new LightControllerPWMChannel(QString::number(i+1), this);
                        get_pwmChannels()->append(channel);
                        channel->set_value(data.mid(1 + i*2, 2).toInt(nullptr, 16));
                        connect(channel, &LightControllerPWMChannel::valueChanged, this, &LightController::updateDevice);
                    }
                    break;
                default:
                    // 1 x PWM + 1 x RGB
                    update_controllerType(V1_1xPWM_1xRGB);
                    auto pwmChannel = new LightControllerPWMChannel("1", this);
                    get_pwmChannels()->append(pwmChannel);
                    pwmChannel->set_value(data.mid(1, 2).toInt(nullptr, 16));
                    connect(pwmChannel, &LightControllerPWMChannel::valueChanged, this, &LightController::updateDevice);

                    auto rgbChannel = new LightControllerRGBChannel("2", this);
                    get_rgbChannels()->append(rgbChannel);
                    rgbChannel->set_redValue(data.mid(3, 2).toInt(nullptr, 16));
                    rgbChannel->set_greenValue(data.mid(5, 2).toInt(nullptr, 16));
                    rgbChannel->set_blueValue(data.mid(7, 2).toInt(nullptr, 16));
                    connect(rgbChannel, &LightControllerRGBChannel::redValueChanged, this, &LightController::updateDevice);
                    connect(rgbChannel, &LightControllerRGBChannel::greenValueChanged, this, &LightController::updateDevice);
                    connect(rgbChannel, &LightControllerRGBChannel::blueValueChanged, this, &LightController::updateDevice);
                    break;
                }
            } else if(m_command.startsWith("UV")) {
                // TODO: implement read scene response parser
            } else if(m_command.startsWith("UI")) {
                // TODO: implement invoke scene response parser
            }
        }
    }

    update_isBusy(false);
}

void LightController::updateDevice()
{
    QByteArray command = updateDeviceCommand();

    if(command.length() != 13) {
        qCritical() << "ERROR: invalid command length: 13 !=" << command.length() << "command:" << command;
        return;
    }

    if (m_command != command) {
        qDebug() << "sending update to device:" << command;
        writeToDevice(command);
    }
}

bool LightController::writeToDevice(const QByteArray &data)
{
    auto writeCharacteristic = m_service->characteristic(writeCharacteristicUuid);
    if (!writeCharacteristic.isValid()) {
        update_message("Cannot get write characteristic");
        qDebug() << "cannot get write characteristic";
        return false;
    }

    m_command = data;
    m_service->writeCharacteristic(writeCharacteristic, m_command,  QLowEnergyService::WriteMode::WriteWithoutResponse);

    return true;
}

} // namespace il
