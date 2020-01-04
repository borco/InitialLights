#include "config.h"

#include "../jsonhelper.h"

#include <QJsonObject>

namespace il {
namespace gui {

namespace {
const bool defaultIsDiscoveringPeriodically { false };
const QString jsonIsDiscoveringPeriodicallyTag { "isDiscoveringPeriodically"};
}

Config::Config(QObject *parent)
    : QObject(parent)
    , m_isDiscoveringPeriodically(defaultIsDiscoveringPeriodically)
{
}

Config::~Config()
{
}

void Config::read(const QJsonObject &json, const QString &tag)
{
    if(!json.contains(tag))
        return;

    QJsonValue localValue = json[tag];

    if (!localValue.isObject())
        return;

    QJsonObject localJson = localValue.toObject();

    READ_PROPERTY_IF_EXISTS(bool, localJson, jsonIsDiscoveringPeriodicallyTag, isDiscoveringPeriodically)
}

void Config::write(QJsonObject &json, const QString &tag) const
{
    QJsonObject localJson;
    localJson[jsonIsDiscoveringPeriodicallyTag] = m_isDiscoveringPeriodically;
    json[tag] = localJson;
}

void Config::clearLocalData()
{
    set_isDiscoveringPeriodically(defaultIsDiscoveringPeriodically);
}

} // namespace gui
} // namespace il
