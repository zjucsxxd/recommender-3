// Copyright (c) 2014, Alibaba Inc.
// All right reserved.
//
// Author: DongPing HUANG <dongping.huang@alibaba-inc.com>
// Created: 2014/02/08
// Description:

#ifndef STONE_CONSENSUS_RAFT_TEST_MOCK_RAFT_ROLE_LISTENER_H
#define STONE_CONSENSUS_RAFT_TEST_MOCK_RAFT_ROLE_LISTENER_H

#include "gmock/gmock.h"
#include "stone/consensus/raft/raft_role_fwd.h"

namespace apsara {
namespace stone {
namespace raft {

class MockRaftRoleListener : public RaftRoleListener
{
public:
    virtual MOCK_METHOD1(OnRaftRoleChanged, void(State));
};

} // namespace raft
} // namespace stone
} // namespace apsara

#endif // STONE_CONSENSUS_RAFT_TEST_MOCK_RAFT_ROLE_LISTENER_H
