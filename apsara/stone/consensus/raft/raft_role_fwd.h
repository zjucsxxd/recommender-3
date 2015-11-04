// Copyright (c) 2013, Alibaba Inc.
// All right reserved.
//
// Author: DongPing HUANG <dongping.huang@alibaba-inc.com>
// Created: 2013/12/12
// Description:

#ifndef STONE_CONSENSUS_RAFT_RAFT_ROLE_FWD_H
#define STONE_CONSENSUS_RAFT_RAFT_ROLE_FWD_H

#include "stone/consensus/raft/raft_fwd.h"

namespace apsara {
namespace stone {
namespace raft {

class RaftRoleListener
{
public:
    virtual ~RaftRoleListener() {}
    virtual void OnRaftRoleChanged(State state) = 0;
};

} // namespace raft
} // namespace stone
} // namespace apsara

#endif // STONE_CONSENSUS_RAFT_RAFT_ROLE_FWD_H
