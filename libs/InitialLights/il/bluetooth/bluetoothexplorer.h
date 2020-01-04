#pragma once

#include "ibluetoothexplorer.h"

#include <QBluetoothDeviceDiscoveryAgent>
#include <QSet>

namespace il {

namespace controllers {
class Controller;
class ControllerCollection;
}

namespace bluetooth {

class INITIALLIGHTSSHARED_EXPORT BluetoothExplorer : public IBluetoothExplorer
{
    Q_OBJECT

public:
    explicit BluetoothExplorer(controllers::ControllerCollection* controllers, QObject *parent = nullptr);

public slots:
    void discover() override;

private:
    void updateOfflineControllers();
    void deviceDiscovered(const QBluetoothDeviceInfo &info);
    void discoveryFailed(QBluetoothDeviceDiscoveryAgent::Error error);
    void discoveryFinished();

    QSet<controllers::Controller*> m_onlineControllers;
    controllers::ControllerCollection* m_controllers;
    QBluetoothDeviceDiscoveryAgent m_deviceDiscoveryAgent;
};

} // namespace bluetooth
} // namespace il
