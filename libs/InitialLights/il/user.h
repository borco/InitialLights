#pragma once

#include "initiallights_global.h"
#include "QQmlAutoPropertyHelpers.h"
#include "QQmlObjectListModel.h"

namespace il {

class INITIALLIGHTSSHARED_EXPORT User : public QObject
{
    Q_OBJECT

    QML_WRITABLE_AUTO_PROPERTY(bool, isLogged)
    QML_READONLY_AUTO_PROPERTY(bool, isOperationPending)

public:
    explicit User(QObject *parent = nullptr);
    virtual ~User();

    void read(const QJsonObject& json, const QString& tag);
    void write(QJsonObject& json, const QString& tag) const;
    void clearLocalData();

signals:
    void loginRequestFinished();

public slots:
    QString validateEmail(QString email) const;
    QString validatePassword(QString password) const;

    void login(const QString& user, const QString& password);
    void resetPassword(const QString& user);
    bool registerNewUser(const QString& user, const QString& password);
    void logout();
};

} // namespace il
