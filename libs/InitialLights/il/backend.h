#pragma once

#include "initiallights_global.h"
#include "QQmlAutoPropertyHelpers.h"
#include "QQmlObjectListModel.h"

namespace il {

class RoomCollection;
class User;

namespace bluetooth {
class BluetoothExplorer;
}

namespace controllers {
class ControllerCollection;
}

namespace gui {
class Config;
}

class INITIALLIGHTSSHARED_EXPORT BackEnd : public QObject
{
    Q_OBJECT

    QML_READONLY_AUTO_PROPERTY(int, version)

    QML_WRITABLE_AUTO_PROPERTY(bool, showOnboarding)
    QML_WRITABLE_AUTO_PROPERTY(bool, showInitialSetup)

    Q_PROPERTY(il::bluetooth::BluetoothExplorer* bluetoothExplorer READ bluetoothExplorer CONSTANT)
    Q_PROPERTY(il::controllers::ControllerCollection* controllers READ controllers CONSTANT)
    Q_PROPERTY(il::gui::Config* guiConfig READ guiConfig CONSTANT)
    Q_PROPERTY(il::RoomCollection* rooms READ rooms CONSTANT)
    Q_PROPERTY(il::User* user READ user CONSTANT)

public:
    explicit BackEnd(QObject *parent = nullptr);
    ~BackEnd() override;

public slots:
    void clearLocalData();
    void loadLocalData();
    void saveLocalData();

    bluetooth::BluetoothExplorer* bluetoothExplorer() const { return m_bluetoothExplorer; }
    controllers::ControllerCollection* controllers() const { return m_controllers; }
    gui::Config* guiConfig() const { return m_guiConfig; }
    RoomCollection* rooms() const { return m_rooms; }
    User* user() const { return m_user; }

private:
    void read(const QJsonObject& json);
    void write(QJsonObject& json) const;

    bluetooth::BluetoothExplorer* m_bluetoothExplorer;
    controllers::ControllerCollection* m_controllers;
    gui::Config* m_guiConfig;
    RoomCollection* m_rooms;
    User* m_user;
};

} // namespace il
