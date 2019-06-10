#pragma once

#include "abstractlightcontroller.h"

#include <QLowEnergyController>
#include <QLowEnergyService>
#include <QDateTime>

namespace il {

class DeviceInfo;

class LightController : public AbstractLightController
{
    Q_OBJECT

public:
    explicit LightController(DeviceInfo* info, QObject *parent = nullptr);
    ~LightController() override;

    void clear() override;

public slots:
    void connectToController() override;
    void disconnectFromController() override;

private:
    //QLowEnergyController
    void serviceDiscovered(const QBluetoothUuid &);
    void serviceScanDone();
    void controllerError(QLowEnergyController::Error);
    void controllerConnected();
    void controllerDisconnected();

    //QLowEnergyService
    void serviceStateChanged(QLowEnergyService::ServiceState state);
    void serviceCharacteristicChanged(const QLowEnergyCharacteristic &characteristic,
                              const QByteArray &data);
    void serviceDescriptorWritten(const QLowEnergyDescriptor &descriptor,
                                  const QByteArray &data);
    void serviceError(QLowEnergyService::ServiceError e);

    void updateFromDevice(const QByteArray& data);
    void updateDevice();

    DeviceInfo* m_info;
    QScopedPointer<QLowEnergyController> m_controller;
    QScopedPointer<QLowEnergyService> m_service;
    QLowEnergyDescriptor m_notificationDescriptor;
    QDateTime m_start;
    QDateTime m_stop;
    QByteArray m_currentCommand;
};

} // namespace il
