// Copyright (c) 2013, Alibaba Inc.
// All right reserved.
//
// Author: DongPing HUANG <dongping.huang@alibaba-inc.com>
// Created: 2013/08/09
// Description:

#ifndef STONE_PERSISTENCE_CHANGELOG_RECORD_FILE_ITERATOR_H
#define STONE_PERSISTENCE_CHANGELOG_RECORD_FILE_ITERATOR_H

#include "stone/base/types.h"
#include "stone/persistence/changelog/change_log.pb.h"
#include "stone/persistence/changelog/internal_record_iterator.h"

namespace apsara {
namespace stone {

class RecordReader;

namespace changelog {

class RecordFileIterator : public InternalRecordIterator
{
public:
    RecordFileIterator(StreamReader* streamReader, RecordReader* reader);
    ~RecordFileIterator();

    void Next();
    bool Valid();
    uint64_t SeqNum();
    void Record(Message* message);

private:
    StreamReader* mStreamReader;
    RecordReader* mReader;
    changelog::Record mRecord;
    bool mValid;
};

} // namespace changelog
} // namespace stone
} // namespace apsara

#endif // STONE_PERSISTENCE_CHANGELOG_RECORD_FILE_ITERATOR_H

