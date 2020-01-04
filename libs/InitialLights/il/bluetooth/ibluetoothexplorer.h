#pragma once

#include "initiallights_global.h"
#include "QQmlVarPropertyHelpers.h"

namespace il {
namespace bluetooth {

class INITIALLIGHTSSHARED_EXPORT IBluetoothExplorer : public QObject
{
    Q_OBJECT

    QML_READONLY_VAR_PROPERTY(bool, isDiscovering)
    QML_WRITABLE_VAR_PROPERTY(QString, message)
    QML_WRITABLE_VAR_PROPERTY(int, discoverTimeout)

public:
    virtual ~IBluetoothExplorer();

signals:
    void discoverFinished();

public slots:
    virtual void discover() = 0;

protected:
    explicit IBluetoothExplorer(QObject *parent = nullptr);
};

} // namespace bluetooth
} // namespace il
