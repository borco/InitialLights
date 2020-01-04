#pragma once

#include "initiallights_global.h"
#include "QQmlAutoPropertyHelpers.h"

namespace il {
namespace gui {

class Config : public QObject
{
    Q_OBJECT

    QML_WRITABLE_AUTO_PROPERTY(bool, isDiscoveringPeriodically)

public:
    explicit Config(QObject *parent = nullptr);
    ~Config() override;

    void read(const QJsonObject& json, const QString& tag);
    void write(QJsonObject& json, const QString& tag) const;
    void clearLocalData();
};

} // namespace gui
} // namespace il
