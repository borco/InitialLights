#include "controllercollection.h"

#include "controller.h"
#include "../bluetooth/bluetoothcontroller.h"
#include "../jsonhelper.h"

#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>

namespace il {

template<>
void readCollectionPropertyIfExists<controllers::Controller>(const QJsonObject& json, const QString& tag, QQmlObjectListModel<controllers::Controller>* property)
{
    if (json.contains(tag) && json[tag].isArray()) {
        QJsonArray jsonArray = json[tag].toArray();
        property->clear();
        if (jsonArray.empty()) return;
        for (int i = 0; i < jsonArray.size(); ++i) {
            QJsonObject jsonObject = jsonArray[i].toObject();
            auto bluetoothController = new bluetooth::BluetoothController;
            controllers::Controller* object = new controllers::Controller(bluetoothController);
            object->read(jsonObject);
            property->append(object);
        }
    }
}

namespace controllers {

ControllerCollection::ControllerCollection(QObject *parent)
    : QObject(parent)
    , m_items { new QQmlObjectListModel<Controller>(this) }
{
}

ControllerCollection::~ControllerCollection()
{
}

void ControllerCollection::read(const QJsonObject &json, const QString &tag)
{
    readCollectionPropertyIfExists<Controller>(json, tag, m_items);
    std::for_each (m_items->begin(), m_items->end(), [this](Controller* controller) {
        connect(controller, &Controller::kindChanged, this, &ControllerCollection::onControllerKindChanged);
    });
}

void ControllerCollection::write(QJsonObject &json, const QString &tag) const
{
    writeCollectionProperty(json, tag, m_items);
}

void ControllerCollection::clearLocalData()
{
    m_items->clear();
}

Controller *ControllerCollection::appendNewController(bluetooth::IBluetoothController *bluetoothController)
{
    Controller* controller = new Controller(bluetoothController);
    m_items->append(controller);
    connect(controller, &Controller::kindChanged, this, &ControllerCollection::onControllerKindChanged);
    return controller;
}

void ControllerCollection::onControllerKindChanged()
{
    Controller* controller = qobject_cast<Controller*>(sender());
    if (!controller)
        return;
    controllerKindChanged(controller);
}

} // namespace controllers
} // namespace il
