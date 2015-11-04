// Copyright (c) 2014, Alibaba Inc.
// All right reserved.
//
// Author: DongPing HUANG <dongping.huang@alibaba-inc.com>
// Created: 2014/01/22
// Description:

#ifndef STONE_CONSENSUS_RAFT_TEST_MOCK_RAFT_SERVICE_H
#define STONE_CONSENSUS_RAFT_TEST_MOCK_RAFT_SERVICE_H

#include "gmock/gmock.h"
#include "stone/consensus/raft/raft.pb.h"

namespace apsara {
namespace stone {
namespace raft {

class MockRaftService : public RaftService
{
public:
    MOCK_METHOD4(RequestVote, void(RpcController* ,
                                   const RequestVote_Request*,
                                   RequestVote_Response*,
                                   ::google::protobuf::Closure*));
    MOCK_METHOD4(QueryLogId, void(RpcController* ,
                                  const QueryLogId_Request*,
                                  QueryLogId_Response*,
                                  ::google::protobuf::Closure*));
    MOCK_METHOD4(AppendEntry, void(RpcController*,
                                   const AppendEntry_Request*,
                                   AppendEntry_Response*,
                                   ::google::protobuf::Closure*));
    MOCK_METHOD4(AppendBlob, void(RpcController*,
                                  const AppendBlob_Request*,
                                  AppendBlob_Response*,
                                  ::google::protobuf::Closure*));
};

} // namespace raft
} // namespace stone
} // namespace apsara

#endif // STONE_CONSENSUS_RAFT_TEST_MOCK_RAFT_SERVICE_H
