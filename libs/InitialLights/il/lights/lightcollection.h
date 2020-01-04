#pragma once

#include "initiallights_global.h"
#include "lightkind.h"

#include "QQmlAutoPropertyHelpers.h"
#include "QQmlObjectListModel.h"

namespace il {

namespace controllers {
class Controller;
}

namespace lights {

class ILight;

class INITIALLIGHTSSHARED_EXPORT LightCollection : public QObject
{
    Q_OBJECT

    QML_OBJMODEL_PROPERTY(il::lights::ILight, items)

public:
    explicit LightCollection(controllers::Controller* controller, QObject *parent = nullptr);
    ~LightCollection() override;

    void read(const QJsonObject& json);
    void write(QJsonObject& json) const;
    void clearLocalData();

    ILight *appendNewLight(LightKind::Kind kind);

private:
    controllers::Controller* m_controller;
};

} // namespace lights
} // namespace il
