// Copyright (c) 2014, Alibaba Inc.
// All right reserved.
//
// Author: DongPing HUANG <dongping.huang@alibaba-inc.com>
// Created: 2014/01/17
// Description:

#ifndef STONE_CONSENSUS_RAFT_TEST_MOCK_RECORD_ITERATOR_H
#define STONE_CONSENSUS_RAFT_TEST_MOCK_RECORD_ITERATOR_H

#include <list>
#include "gmock/gmock.h"
#include "stone/persistence/changelog/change_log.h"

namespace apsara {
namespace stone {
namespace raft {

class MockRecordIterator : public RecordIterator
{
public:
    MOCK_METHOD0(Next, void(void));
    MOCK_METHOD0(Valid, bool(void));
    MOCK_METHOD1(Record, void(Message* msg));
};

class MockRaftRecordIterator : public RaftRecordIterator
{
public:
    MockRaftRecordIterator() : RaftRecordIterator(NULL) {}
    MOCK_METHOD0(Next, void(void));
    MOCK_METHOD0(Valid, bool(void));
    MOCK_METHOD1(Record, void(Message* msg));
    MOCK_CONST_METHOD0(GetCommittedId, raft::LogId(void));
    MOCK_CONST_METHOD0(GetLastId, raft::LogId(void));
    MOCK_CONST_METHOD0(GetPending, std::list<raft::LogId>(void));
};

} // namespace raft
} // namespace stone
} // namespace apsara

#endif // STONE_CONSENSUS_RAFT_TEST_MOCK_RECORD_ITERATOR_H
