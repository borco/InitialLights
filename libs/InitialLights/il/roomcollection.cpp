#include "roomcollection.h"

#include "iindexer.h"
#include "jsonhelper.h"
#include "room.h"

#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>

namespace il {

RoomCollection::RoomCollection(std::function<IIndexer* (IIndexed *, QObject *)> indexerAllocator, QObject *parent)
    : QObject(parent)
    , m_items { new QQmlObjectListModel<Room>(this) }
    , m_indexer { indexerAllocator(this, this) }
{
}

RoomCollection::~RoomCollection()
{
}

int RoomCollection::count() const
{
    return m_items->count();
}

int RoomCollection::maxIndex() const
{
    auto mi = std::max_element(m_items->begin(), m_items->end(), [](auto lhs, auto rhs) { return lhs->rid() < rhs->rid(); });
    return mi != m_items->end() ? (*mi)->rid() : 0;
}

std::vector<int> RoomCollection::indexes() const
{
    std::vector<int> ret;
    ret.resize(std::vector<int>::size_type(m_items->count()));
    std::transform(m_items->begin(), m_items->end(), ret.begin(), [](Room* room) { return room->rid(); });
    return ret;
}

void RoomCollection::read(const QJsonObject &json, const QString& tag)
{
    readCollectionPropertyIfExists<Room>(json, tag, m_items);
    m_indexer->rebuild();
}

void RoomCollection::write(QJsonObject &json, const QString &tag) const
{
    writeCollectionProperty(json, tag, m_items);
}

void RoomCollection::clearLocalData()
{
    m_items->clear();
}

void RoomCollection::appendNewRoom()
{
    Room* room = new Room();
    room->set_rid(m_indexer->allocateNextFreeIndex());
    m_items->append(room);
}

void RoomCollection::removeRoom(int position)
{
    auto room = m_items->at(position);
    int rid = room->rid();
    m_items->remove(position);
    m_indexer->freeIndex(rid);
}

} // namespace il
