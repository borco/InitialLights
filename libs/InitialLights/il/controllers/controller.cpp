#include "controller.h"

#include "../bluetooth/bluetoothcontroller.h"
#include "../jsonhelper.h"
#include "../lights/analogiclight.h"
#include "../lights/lightcollection.h"

#include <QJsonObject>

namespace il {

namespace {
const QString jsonNameTag { "name" };
const QString jsonAddressTag { "address" };
const QString jsonIsEnabledTag { "isEnabled" };
const QString jsonKindTag { "kind" };

void readIfExists(const QJsonObject &json, const QString &tag, controllers::Controller::Kind &out)
{
    if (json.contains(tag) && json[tag].isString()) {
        int newValue = QMetaEnum::fromType<controllers::Controller::Kind>().keyToValue(json[tag].toString().toStdString().c_str());
        if (newValue >= 0) {
            out = controllers::Controller::Kind(newValue);
        }
    }
}

}

namespace controllers {

Controller::Controller(bluetooth::IBluetoothController *bluetoothController, QObject *parent)
    : QObject(parent)
    , m_isOnline { false }
    , m_isEnabled { false }
    , m_kind { Unknown }
    , m_lights { new lights::LightCollection(this, this) }
    , m_bluetoothController { bluetoothController }
{
    bluetoothController->setParent(this);
    connect(bluetoothController, &bluetooth::IBluetoothController::isConnectedChanged, this, &Controller::bluetoothControllerIsConnectedChanged);
}

Controller::~Controller()
{
}

void Controller::read(const QJsonObject &json)
{
    READ_PROPERTY_IF_EXISTS(QString, json, jsonNameTag, name)
    READ_PROPERTY_IF_EXISTS(QString, json, jsonAddressTag, address)
    READ_PROPERTY_IF_EXISTS(bool, json, jsonIsEnabledTag, isEnabled)
    READ_PROPERTY_IF_EXISTS(Controller::Kind, json, jsonKindTag, kind)

    m_lights->read(json);
}

void Controller::write(QJsonObject &json) const
{
    json[jsonNameTag] = m_name;
    json[jsonAddressTag] = m_address;
    json[jsonIsEnabledTag] = m_isEnabled;
    json[jsonKindTag] = QMetaEnum::fromType<Kind>().valueToKey(m_kind);

    m_lights->write(json);
}

void Controller::set_kind(Controller::Kind kind)
{
    if (m_kind == kind)
        return;

    m_kind = kind;
    emit kindChanged(m_kind);

    m_lights->clearLocalData();
    switch (m_kind) {
    case RGBW: {
        lights::ILight* light;
        light = m_lights->appendNewLight(lights::LightKind::RGBW);
        light->set_name("RGBW");
        break;
    }
    case Analogic: {
        lights::AnalogicLight* light;
        light = qobject_cast<lights::AnalogicLight*>(m_lights->appendNewLight(lights::LightKind::Analogic));
        light->set_name("Analogic #1");
        light->set_channel(lights::AnalogicLight::Channel1);
        light = qobject_cast<lights::AnalogicLight*>(m_lights->appendNewLight(lights::LightKind::Analogic));
        light->set_name("Analogic #2");
        light->set_channel(lights::AnalogicLight::Channel2);
        break;
    }
    case Unknown:
        break;
    }
}

void Controller::bluetoothControllerIsConnectedChanged()
{
    if (!m_bluetoothController->isConnected()) {
        set_isOnline(false);
    }
}

} // namespace controllers
} // namespace il
