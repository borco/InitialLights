#include "backend.h"

#include "bluetooth/bluetoothexplorer.h"
#include "controllers/controller.h"
#include "controllers/controllercollection.h"
#include "gui/config.h"
#include "jsonhelper.h"
#include "roomcollection.h"
#include "simpleindexer.h"
#include "user.h"

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QStandardPaths>

namespace il {

namespace  {

const QString jsonShowOnboardingTag { "showOnboarding" };
const QString jsonShowInitialSetupTag { "showInitialSetup" };

const QString jsonControllersTag { "controllers" };
const QString jsonGuiConfigTag { "guiConfig" };
const QString jsonUserTag { "user" };

QString localDataDirName()
{
    return QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
}

QString localDataFileName()
{
    return localDataDirName() + "/config.json";
}

bool createPathIfNeeded(const QString& path) {
    return QDir().mkpath(path);
}

}

BackEnd::BackEnd(QObject *parent)
    : QObject(parent)
    , m_version { 1 }
    , m_showOnboarding { true }
    , m_showInitialSetup { true }
    , m_user { new User(this) }
{
    auto indexerAllocator = [](IIndexed* indexed, QObject* parent) { return new SimpleIndexer(indexed, parent); };
    m_controllers = new controllers::ControllerCollection(this);
    m_rooms = new RoomCollection(indexerAllocator, this);
    m_bluetoothExplorer = new bluetooth::BluetoothExplorer(m_controllers, this);

    m_guiConfig = new gui::Config(this);
}

BackEnd::~BackEnd()
{
}

void BackEnd::loadLocalData()
{
    auto loadFileName = localDataFileName();

    QFile loadFile(loadFileName);
    if (!loadFile.open(QIODevice::ReadOnly)) {
        qWarning() << "Couldn't open save file:" << loadFile.fileName();

        loadFile.setFileName(":/DemoConfig.json");
        if(!loadFile.open(QIODevice::ReadOnly)) {
            qWarning() << "Couldn't open demo config file:" << loadFile.fileName();
            return;
        }
    }

    qDebug() << "loading local data from:" << loadFile.fileName();

    QByteArray saveData { loadFile.readAll() };
    QJsonDocument loadDoc { QJsonDocument::fromJson(saveData) };

    read(loadDoc.object());
}

void BackEnd::saveLocalData()
{
    createPathIfNeeded(localDataDirName());

    auto saveFileName = localDataFileName();

    QFile saveFile(saveFileName);
    if (!saveFile.open(QIODevice::WriteOnly)) {
        qWarning() << "Couldn't open save file:" << saveFileName;
        return;
    }

    qDebug() << "saving local data to:" << saveFileName;

    QJsonObject backendObject;
    write(backendObject);
    QJsonDocument saveDoc(backendObject);
    saveFile.write(saveDoc.toJson());
}

void BackEnd::read(const QJsonObject &json)
{
    READ_PROPERTY_IF_EXISTS(bool, json, jsonShowOnboardingTag, showOnboarding)
    READ_PROPERTY_IF_EXISTS(bool, json, jsonShowInitialSetupTag, showInitialSetup)

    m_controllers->read(json, jsonControllersTag);
    m_rooms->read(json);
    m_user->read(json, jsonUserTag);
    m_guiConfig->read(json, jsonGuiConfigTag);
}

void BackEnd::write(QJsonObject &json) const
{
    json[jsonShowOnboardingTag] = m_showOnboarding;
    json[jsonShowInitialSetupTag] = m_showInitialSetup;

    m_controllers->write(json, jsonControllersTag);
    m_rooms->write(json);
    m_user->write(json, jsonUserTag);
    m_guiConfig->write(json, jsonGuiConfigTag);
}

void BackEnd::clearLocalData()
{
    qDebug() << "clearing local data...";

    set_showOnboarding(true);
    set_showInitialSetup(true);

    m_controllers->clearLocalData();
    m_rooms->clearLocalData();
    m_user->clearLocalData();

    QFile::remove(localDataFileName());
}

} // namespace il
