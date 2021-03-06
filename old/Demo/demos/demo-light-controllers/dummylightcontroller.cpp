#include "dummylightcontroller.h"
#include "lightcontrollerpwmchannel.h"
#include "lightcontrollerrgbchannel.h"
#include "lightcontrollervoltagechannel.h"

#include <QTimer>

namespace il {

DummyLightController::DummyLightController(ControllerType controllerType,
                                           const QString &name,
                                           const QString &address,
                                           QObject *parent)
    : AbstractLightController (parent)
    , m_connectDelay { 1000 }
{
    update_controllerType(controllerType);
    update_name(name);
    update_address(address);
}

void DummyLightController::connectToController()
{
    clear();
    update_isBusy(true);
    if (m_connectDelay > 0) {
        QTimer::singleShot(m_connectDelay, this, &DummyLightController::connectToControllerFinished);
    } else {
        connectToControllerFinished();
    }
}

void DummyLightController::disconnectFromController()
{
    update_isConnected(false);
    clear();
}

void DummyLightController::connectToControllerFinished()
{
    switch (controllerType()) {
    case V1_4xPWM:
        for (int i = 0; i < 4; ++i) {
            get_pwmChannels()->append(new LightControllerPWMChannel(QString::number(i+1), this));
        }
        break;
    case V1_1xPWM_1xRGB:
        get_pwmChannels()->append(new LightControllerPWMChannel("1", this));
        get_rgbChannels()->append(new LightControllerRGBChannel("2", this));
        break;
    case V1_2x10V:
        for (int i = 0; i < 2; ++i) {
            get_voltageChannels()->append(new LightControllerVoltageChannel(QString::number(i+1), this));
        }
        break;
    default:
        break;
    }
    update_isBusy(false);
    update_isConnected(true);
}

} // namespace il
