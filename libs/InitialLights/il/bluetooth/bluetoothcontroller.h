#pragma once

#include "ibluetoothcontroller.h"

#include <QLowEnergyController>

namespace il {
namespace bluetooth {

class INITIALLIGHTSSHARED_EXPORT BluetoothController : public IBluetoothController
{
    Q_OBJECT

public:
    explicit BluetoothController(QObject *parent = nullptr);
    ~BluetoothController() override;

    void connectToController(const QBluetoothDeviceInfo& info) override;
    void disconnectFromController() override;
    void lightWriteWithoutResponse(Light light, const QByteArray &data) override;

private:
    //QLowEnergyController
    void controllerConnected();
    void controllerDisconnected();
    void controllerError(QLowEnergyController::Error error);
    void serviceDiscovered(const QBluetoothUuid &uuid);
    void serviceScanDone();

    void resetService(QScopedPointer<QLowEnergyService>& service, const QBluetoothUuid &uuid, const QString& serviceName);
    void discoverLightsService();
    void discoverButtonsService();
    void checkServiceDiscoveryFinished();

    //Lights QLowEnergyService
    void lightsServiceStateChanged(QLowEnergyService::ServiceState state);
    void lightsServiceCharacteristicChanged(const QLowEnergyCharacteristic &characteristic,
                                      const QByteArray &data);
    void lightsServiceDescriptorWritten(const QLowEnergyDescriptor &descriptor,
                                  const QByteArray &data);
    void lightsServiceError(QLowEnergyService::ServiceError e);

    QScopedPointer<QLowEnergyController> m_controller;
    QScopedPointer<QLowEnergyService> m_lightsService;
    QScopedPointer<QLowEnergyService> m_buttonsService;
    QList<QLowEnergyDescriptor> m_lightNotificationDescriptors;

    bool m_isDiscoveringLightsService { false };
    bool m_isDiscoveringButtonsService { false };
};

} // namespace bluetooth
} // namespace il
