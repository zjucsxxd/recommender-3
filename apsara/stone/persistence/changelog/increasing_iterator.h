// Copyright (c) 2013, Alibaba Inc.
// All right reserved.
//
// Author: DongPing HUANG <dongping.huang@alibaba-inc.com>
// Created: 2013/08/09
// Description:

#ifndef STONE_PERSISTENCE_CHANGELOG_INCREASING_ITERATOR_H
#define STONE_PERSISTENCE_CHANGELOG_INCREASING_ITERATOR_H

#include "stone/persistence/changelog/internal_record_iterator.h"
#include "stone/persistence/changelog/version.h"

namespace apsara {
namespace stone {
namespace changelog {

class IncreasingIterator : public RecordIterator
{
public:
    IncreasingIterator(InternalRecordIterator* iterator, Version* version);
    ~IncreasingIterator();

    void Next();
    bool Valid();
    void Record(Message* message);

private:
    Version* mVersion;
    InternalRecordIterator* mInternalIterator;
    uint64_t mSeqNum;
};

} // namespace changelog
} // namespace stone
} // namespace apsara

#endif // STONE_PERSISTENCE_CHANGELOG_INCREASING_ITERATOR_H
