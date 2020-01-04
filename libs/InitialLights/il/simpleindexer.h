#pragma once

#include "initiallights_global.h"
#include "iindexer.h"

#include <deque>

#include <QObject>

namespace il {

class IIndexed;

class INITIALLIGHTSSHARED_EXPORT SimpleIndexer : public QObject, public IIndexer
{
    Q_OBJECT
public:
    explicit SimpleIndexer(IIndexed* indexed, QObject *parent = nullptr);
    ~SimpleIndexer() override;

    int allocateNextFreeIndex() override;
    void freeIndex(int index) override;
    void rebuild() override;

private:
    IIndexed* m_indexed { nullptr };
    std::deque<int> m_unusedIndexes;
};

}
