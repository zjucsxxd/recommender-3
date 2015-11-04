// Copyright (c) 2014, Alibaba Inc.
// All right reserved.
//
// Author: DongPing HUANG <dongping.huang@alibaba-inc.com>
// Created: 2014/01/26
// Description:

#ifndef STONE_CONSENSUS_RAFT_TEST_MOCK_RAFT_SERVER_H
#define STONE_CONSENSUS_RAFT_TEST_MOCK_RAFT_SERVER_H

#include "gmock/gmock.h"
#include "stone/consensus/raft/raft_reader.h"
#include "stone/consensus/raft/test/raft_test_util.h"

namespace apsara {
namespace stone {
namespace raft {

class MockRaftServer : public RaftServer
{
public:
    MOCK_METHOD2(SetCkptBlob, void(const char*, size_t));

    virtual google::protobuf::Message* CreateCkptRecordInstance()
    {
        return new SimpleMessage();
    }

    MOCK_METHOD1(AppendCkptRecord, void(const google::protobuf::Message*));
    MOCK_METHOD1(AppendLogRecord, void(const google::protobuf::Message*));

    virtual google::protobuf::Message* CreateLogRecordInstance()
    {
        return new SimpleMessage();
    }
};

} // namespace raft
} // namespace stone
} // namespace apsara

#endif // STONE_CONSENSUS_RAFT_TEST_MOCK_RAFT_SERVER_H
