#pragma once

#include "initiallights_global.h"
#include "iindexed.h"

#include "QQmlAutoPropertyHelpers.h"
#include "QQmlObjectListModel.h"

namespace il {

class IIndexer;
class Room;

class INITIALLIGHTSSHARED_EXPORT RoomCollection : public QObject, public IIndexed
{
    Q_OBJECT

    QML_OBJMODEL_PROPERTY(il::Room, items)

public:
    explicit RoomCollection(std::function<IIndexer* (IIndexed *, QObject*)> indexerAllocator, QObject *parent = nullptr);
    ~RoomCollection() override;

    int count() const override;
    int maxIndex() const override;
    std::vector<int> indexes() const override;

    void read(const QJsonObject& json, const QString& tag);
    void write(QJsonObject& json, const QString& tag) const;
    void clearLocalData();

public slots:
    void appendNewRoom();
    void removeRoom(int position);

private:
    int maxRoomIndex() const;
    IIndexer* m_indexer;
};

} // namespace il

