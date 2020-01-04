#include "bluetoothcontroller.h"

namespace il {

namespace  {
const QBluetoothUuid lightsUuidService(QStringLiteral("F0001110-0451-4000-B000-000000000000"));
const QBluetoothUuid buttonsUuidService(QStringLiteral("F0001120-0451-4000-B000-000000000000"));

const QMap<bluetooth::IBluetoothController::Light, QBluetoothUuid> lightUuidCharacteristics {
    { bluetooth::IBluetoothController::Light1, QBluetoothUuid(QStringLiteral("F0001111-0451-4000-B000-000000000000")) },
    { bluetooth::IBluetoothController::Light2, QBluetoothUuid(QStringLiteral("F0001112-0451-4000-B000-000000000000")) },
    { bluetooth::IBluetoothController::Light3, QBluetoothUuid(QStringLiteral("F0001113-0451-4000-B000-000000000000")) },
    { bluetooth::IBluetoothController::Light4, QBluetoothUuid(QStringLiteral("F0001114-0451-4000-B000-000000000000")) },
    };
}

namespace bluetooth {

BluetoothController::BluetoothController(QObject* parent)
    : IBluetoothController(parent)
{
}

BluetoothController::~BluetoothController()
{
}

void BluetoothController::connectToController(const QBluetoothDeviceInfo& info)
{
    if (isConnected() || isConnecting())
        return;

    update_isConnecting(true);

    m_controller.reset(new QLowEnergyController(info, this));

    connect(m_controller.get(), &QLowEnergyController::connected,
            this, &BluetoothController::controllerConnected);
    connect(m_controller.get(), &QLowEnergyController::disconnected,
            this, &BluetoothController::controllerDisconnected);
    connect(m_controller.get(), qOverload<QLowEnergyController::Error>(&QLowEnergyController::error),
            this, &BluetoothController::controllerError);
    connect(m_controller.get(), &QLowEnergyController::serviceDiscovered,
            this, &BluetoothController::serviceDiscovered);
    connect(m_controller.get(), &QLowEnergyController::discoveryFinished,
            this, &BluetoothController::serviceScanDone);

    m_controller->connectToDevice();
}

void BluetoothController::disconnectFromController()
{
    if (!isConnected())
        return;

    qDebug() << "+++ forcing bluetooth controller to disconnect";

    controllerDisconnected();
}

void BluetoothController::lightWriteWithoutResponse(IBluetoothController::Light light, const QByteArray &data)
{
    if (!isConnected()) {
        return;
    }

    if (m_lightsService.isNull() || m_lightsService->state() != QLowEnergyService::ServiceDiscovered) {
        qDebug() << "+++ invalid lights service found when trying to write without response";
        return;
    }

    auto characteristic = m_lightsService->characteristic(lightUuidCharacteristics[light]);
    if (!characteristic.isValid()) {
        update_message("Cannot get write characteristic");
        qDebug() << "+++ cannot get write characteristic for" << light;
        return;
    }

    qDebug() << "+++ writing to light" << characteristic.uuid() << ":" << data.toHex().toUpper();
    m_lightsService->writeCharacteristic(characteristic, data,  QLowEnergyService::WriteMode::WriteWithoutResponse);
}

void BluetoothController::controllerConnected()
{
    qDebug() << "+++ connected to controller; discovering services...";
    m_controller->discoverServices();
}

void BluetoothController::controllerDisconnected()
{
    update_message("Controller disconnected");
    qWarning() << "+++ controller disconnected";

    m_buttonsService.reset();
    m_lightsService.reset();
    m_controller.reset();

    update_isConnecting(false);
    update_isConnected(false);
}

void BluetoothController::controllerError(QLowEnergyController::Error error)
{
    update_message("Cannot connect to controller");
    qWarning() << "+++ Controller Error:" << error;
}

void BluetoothController::serviceDiscovered(const QBluetoothUuid &uuid)
{
    qDebug() << "+++ bluetooth controller::service discovered:" << uuid;

    if (uuid == lightsUuidService) {
        resetService(m_lightsService, uuid, "lights service");
    } else if (uuid == buttonsUuidService) {
        resetService(m_buttonsService, uuid, "buttons service");
    }
}

void BluetoothController::serviceScanDone()
{
    qDebug() << "+++ bluetooth controller::service scan done";

    m_isDiscoveringLightsService = !m_lightsService.isNull();
    m_isDiscoveringButtonsService = !m_buttonsService.isNull();

    discoverLightsService();
    discoverButtonsService();

    checkServiceDiscoveryFinished();
}

void BluetoothController::resetService(QScopedPointer<QLowEnergyService> &service, const QBluetoothUuid &uuid, const QString &serviceName)
{
    if (service.isNull()) {
        update_message(serviceName + " found");
        qDebug() << "+++" << serviceName << "found";
        service.reset(m_controller->createServiceObject(uuid, this));
    } else {
        qWarning() << "+++" << serviceName << "already found";
    }
}

void BluetoothController::discoverLightsService()
{
    auto service = m_lightsService.get();
    if (!service) {
        return;
    }

    connect(service, &QLowEnergyService::stateChanged,
            this, &BluetoothController::lightsServiceStateChanged);
    connect(service, &QLowEnergyService::characteristicChanged,
            this, &BluetoothController::lightsServiceCharacteristicChanged);
    connect(service, &QLowEnergyService::descriptorWritten,
            this, &BluetoothController::lightsServiceDescriptorWritten);
    connect(service, qOverload<QLowEnergyService::ServiceError>(&QLowEnergyService::error),
            this, &BluetoothController::lightsServiceError);
    service->discoverDetails();
    update_message("Connecting to service");
    qDebug() << "+++ connecting to service...";
}

void BluetoothController::discoverButtonsService()
{
    m_isDiscoveringButtonsService = false;
    checkServiceDiscoveryFinished();
}

void BluetoothController::checkServiceDiscoveryFinished()
{
    if (!m_isDiscoveringLightsService && !m_isDiscoveringButtonsService) {
        update_isConnecting(false);
    }

    if (!isConnecting()) {
        if (m_lightsService.isNull() && m_buttonsService.isNull()) {
            update_message("No service found");
            qDebug() << "+++ no lights or buttons services found";
        } else {
            update_isConnected(true);
            qDebug() << "+++ lights or buttons services found";
        }
    }
}

void BluetoothController::lightsServiceStateChanged(QLowEnergyService::ServiceState state)
{
    switch (state) {
    case QLowEnergyService::InvalidService: {
        qDebug() << "+++ service state changed" << state;
        update_isConnected(false);
        break;
    }
    case QLowEnergyService::ServiceDiscovered: {
        qDebug() << "+++ service state changed" << state;
        m_isDiscoveringLightsService = false;
        checkServiceDiscoveryFinished();
//        m_lightNotificationDescriptors.clear();
//        for(QBluetoothUuid uuid: lightUuidCharacteristics) {
//            const QLowEnergyCharacteristic characteristic = m_lightsService->characteristic(uuid);
//            if (!characteristic.isValid()) {
//                update_message("Cannot get light characteristic");
//                qDebug() << "cannot get light characteristic:" << uuid;
//                break;
//            }

//            QLowEnergyDescriptor descriptor = characteristic.descriptor(QBluetoothUuid::ClientCharacteristicConfiguration);
//            if (descriptor.isValid()) {
//                m_lightNotificationDescriptors << descriptor;
//            } else {
//                update_message("Cannot set notification descriptor");
//                qWarning() << "cannot set notification descriptor";
//                break;
//            }
//            break;
//        }
        break;
    }
    default:
        qDebug() << "+++ service state changed" << state << "(ignored)";
        //nothing for now
        break;
    }
}

void BluetoothController::lightsServiceCharacteristicChanged(const QLowEnergyCharacteristic &characteristic, const QByteArray &data)
{
    qDebug() << "+++ bluetooth controller::lightsServiceCharacteristicChanged:" << characteristic.uuid() << data;
}

void BluetoothController::lightsServiceDescriptorWritten(const QLowEnergyDescriptor &descriptor, const QByteArray &data)
{
    qDebug() << "+++ bluetooth controller::lightsServiceDescriptorWritten:" << descriptor.uuid() << data;
}

void BluetoothController::lightsServiceError(QLowEnergyService::ServiceError e)
{
    switch (e) {
    case QLowEnergyService::DescriptorWriteError:
        update_message("Cannot obtain service notifications");
        qWarning() << "+++ cannot obtain service notifications";
        break;
    default:
        qWarning() << "+++ service error:" << e;
    }
}

} // namespace bluetooth
} // namespace il
