// Copyright (c) 2013, Alibaba Inc.
// All right reserved.
//
// Author: DongPing HUANG <dongping.huang@alibaba-inc.com>
// Created: 2013/08/09
// Description:

#ifndef STONE_PERSISTENCE_CHANGELOG_INTERNAL_RECORD_ITERATOR_H
#define STONE_PERSISTENCE_CHANGELOG_INTERNAL_RECORD_ITERATOR_H

#include "stone/base/types.h"
#include "stone/persistence/changelog/change_log.pb.h"
#include "stone/persistence/changelog/change_log_reader.h"

namespace apsara {
namespace stone {

class InternalRecordIterator : public RecordIterator
{
public:
    virtual uint64_t SeqNum() = 0;
};

} // namespace stone
} // namespace apsara

#endif // STONE_PERSISTENCE_CHANGELOG_INTERNAL_RECORD_ITERATOR_H

