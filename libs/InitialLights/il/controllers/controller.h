#pragma once

#include "initiallights_global.h"
#include "QQmlAutoPropertyHelpers.h"
#include "QQmlObjectListModel.h"

namespace il {

namespace lights {
class ILight;
class LightCollection;
}

namespace bluetooth {
class IBluetoothController;
}

namespace controllers {

class INITIALLIGHTSSHARED_EXPORT Controller : public QObject
{
public:
    enum Kind {
        RGBW,
        Analogic,
        Unknown
    };

private:
    Q_OBJECT

    QML_WRITABLE_AUTO_PROPERTY(QString, name)
    QML_WRITABLE_AUTO_PROPERTY(QString, address)
    QML_WRITABLE_AUTO_PROPERTY(bool, isOnline)

    QML_WRITABLE_AUTO_PROPERTY(bool, isEnabled)

    Q_ENUM(Kind)
    Q_PROPERTY(il::controllers::Controller::Kind kind READ kind WRITE set_kind NOTIFY kindChanged)

    Q_PROPERTY(il::lights::LightCollection* lights READ lights CONSTANT)

public:
    explicit Controller(bluetooth::IBluetoothController* bluetoothController, QObject *parent = nullptr);
    virtual ~Controller();

    void read(const QJsonObject& json);
    void write(QJsonObject& json) const;

    Kind kind() const { return m_kind; }
    lights::LightCollection* lights() const { return m_lights; }
    bluetooth::IBluetoothController* bluetoothController() const { return m_bluetoothController; }

signals:
    void kindChanged(Kind kind);

public slots:
    void set_kind(Kind kind);

private:
    void bluetoothControllerIsConnectedChanged();

    Kind m_kind;
    lights::LightCollection* m_lights;
    bluetooth::IBluetoothController* m_bluetoothController;
};

} // namespace controllers
} // namespace il
