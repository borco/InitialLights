#include "config.h"

#include "../jsonhelper.h"

#include <QJsonObject>

namespace il {
namespace gui {

namespace {
const bool defaultIsScanningPeriodically { false };
const QString jsonIsScanningPeriodicallyTag { "isScanningPeriodically"};
}

Config::Config(QObject *parent)
    : QObject(parent)
    , m_isScanningPeriodically(defaultIsScanningPeriodically)
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

    READ_PROPERTY_IF_EXISTS(bool, localJson, jsonIsScanningPeriodicallyTag, isScanningPeriodically)
}

void Config::write(QJsonObject &json, const QString &tag) const
{
    QJsonObject localJson;
    localJson[jsonIsScanningPeriodicallyTag] = m_isScanningPeriodically;
    json[tag] = localJson;
}

void Config::clearLocalData()
{
    set_isScanningPeriodically(defaultIsScanningPeriodically);
}

} // namespace gui
} // namespace il
