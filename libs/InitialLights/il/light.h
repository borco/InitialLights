#pragma once

#include "initiallights_global.h"

#include "QQmlAutoPropertyHelpers.h"

namespace il {

class Room;

class INITIALLIGHTSSHARED_EXPORT Light : public QObject
{
public:
    enum LightType {
        UndefinedLightType,
        Analogic,
        PWM,
        RGB,
    };
    Q_ENUM(LightType)

private:
    Q_OBJECT

    QML_READONLY_AUTO_PROPERTY(il::Light::LightType, lightType)

    QML_WRITABLE_AUTO_PROPERTY(QString, name)

    QML_READONLY_AUTO_PROPERTY(int, minValue)
    QML_READONLY_AUTO_PROPERTY(int, maxValue)
    QML_READONLY_AUTO_PROPERTY(int, valueIncrement)

    QML_WRITABLE_AUTO_PROPERTY(int, value)

    QML_WRITABLE_AUTO_PROPERTY(int, redValue)
    QML_WRITABLE_AUTO_PROPERTY(int, greenValue)
    QML_WRITABLE_AUTO_PROPERTY(int, blueValue)

    QML_WRITABLE_AUTO_PROPERTY(double, sideX)
    QML_WRITABLE_AUTO_PROPERTY(double, sideY)

    Q_PROPERTY(QString lightTypeName READ lightTypeName NOTIFY lightTypeChanged)
    Q_PROPERTY(QObject* controller READ controller CONSTANT)
    Q_PROPERTY(il::Room* room READ room WRITE setRoom NOTIFY roomChanged)

public:
    explicit Light(QObject* parent = nullptr);
    explicit Light(LightType lightType, const QString& name, QObject* parent = nullptr);
    virtual ~Light();

    virtual void read(const QJsonObject& json);
    virtual void write(QJsonObject& json) const;

    QString lightTypeName() const;
    QObject* controller() const;
    Room* room() const { return m_room; }

public slots:
    void setRoom(il::Room* room);

signals:
    void lightTypeChanged(QString lightTypeName);
    void roomChanged(il::Room* room);

private:
    static LightType readLightTypeFrom(const QJsonObject& json);
    Room* m_room { nullptr };
};

} // namespace il
