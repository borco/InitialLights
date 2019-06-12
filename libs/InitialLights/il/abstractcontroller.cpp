#include "abstractcontroller.h"
#include "pwmchannel.h"
#include "rgbchannel.h"
#include "analogicchannel.h"

namespace il {

AbstractController::AbstractController(QObject *parent)
    : QObject(parent)
    , m_controllerType { UndefinedControllerType }
    , m_isBusy { false }
    , m_isConnected { false }
    , m_analogicChannels { new QQmlObjectListModel<AnalogicChannel>(this) }
    , m_pwmChannels { new QQmlObjectListModel<PWMChannel>(this) }
    , m_rgbChannels { new QQmlObjectListModel<RGBChannel>(this) }
{
}

AbstractController::~AbstractController()
{
    clear();
}

QByteArray AbstractController::updateDeviceCommand() const
{
    QString command;

    switch (controllerType()) {
    case V1_2x10V:
    {
        Q_ASSERT(m_analogicChannels->size() == 2);
        Q_ASSERT(m_pwmChannels->size() == 0);
        Q_ASSERT(m_rgbChannels->size() == 0);

        auto channels = m_analogicChannels;
        command = QStringLiteral(u"US%1%2%3\n")
                .arg(channels->at(0)->value(), 2, 16, QChar('0'))
                .arg(channels->at(1)->value(), 2, 16, QChar('0'))
                .arg(2, 6, 16, QChar('0'))
                ;
        break;
    }
    case V1_4xPWM: {
        Q_ASSERT(m_analogicChannels->size() == 0);
        Q_ASSERT(m_pwmChannels->size() == 4);
        Q_ASSERT(m_rgbChannels->size() == 0);

        auto channels = m_pwmChannels;
        command = QString("US%1%2%3%4%5\n")
                .arg(channels->at(0)->value(), 2, 16, QChar('0'))
                .arg(channels->at(1)->value(), 2, 16, QChar('0'))
                .arg(channels->at(2)->value(), 2, 16, QChar('0'))
                .arg(channels->at(3)->value(), 2, 16, QChar('0'))
                .arg(3, 2, 16, QChar('0'));
        break;
    }
    case V1_1xPWM_1xRGB: {
        Q_ASSERT(m_analogicChannels->size() == 0);
        Q_ASSERT(m_pwmChannels->size() == 1);
        Q_ASSERT(m_rgbChannels->size() == 1);

        auto pwmChannel = m_pwmChannels->at(0);
        auto rgbChannel = m_rgbChannels->at(0);
        command = QString("US%1%2%3%4%5\n")
                .arg(pwmChannel->value(), 2, 16, QChar('0'))
                .arg(rgbChannel->redValue(), 2, 16, QChar('0'))
                .arg(rgbChannel->greenValue(), 2, 16, QChar('0'))
                .arg(rgbChannel->blueValue(), 2, 16, QChar('0'))
                .arg(1, 2, 16, QChar('0'));
        break;
    }
    default:
        qWarning() << "don't know how to save controller state for controller type:" << controllerType();
        break;
    }

    return command.toUpper().toUtf8();
}

void AbstractController::clear()
{
    clearChannels();
}

void AbstractController::clearChannels()
{
    m_analogicChannels->clear();
    m_pwmChannels->clear();
    m_rgbChannels->clear();
}

} // namespace il
