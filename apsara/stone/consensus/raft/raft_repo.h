// Copyright (c) 2013, Alibaba Inc.
// All right reserved.
//
// Author: DongPing HUANG <dongping.huang@alibaba-inc.com>
// Created: 2013/12/03
// Description:

#ifndef STONE_CONSENSUS_RAFT_RAFT_REPO_H
#define STONE_CONSENSUS_RAFT_RAFT_REPO_H

#include <vector>
#include "stone/base/uncopyable.h"
#include "stone/consensus/raft/peer.h"
#include "stone/consensus/raft/raft_fwd.h"
#include "stone/threading/sync/lock.h"

namespace apsara {
namespace stone {
namespace raft {

class LocalServer;

class RaftRepo : private stone::Uncopyable
{
public:
    typedef stone::RestrictMutex LockType;

    RaftRepo(const RaftOption& option, ChangeLog* changeLog);
    ~RaftRepo();

    LockType* GetLock() { return &mLock; }

    raft::LocalServer* GetLocalServer();

    void GetPeers(std::vector<raft::Peer*>* peers);

    State GetState();

    void StepDown(uint64_t term);

    RaftOption* GetOption();

private:
    LockType mLock;
    raft::LocalServer* mLocalServer;
    std::vector<raft::Peer*> mPeers;
    RaftOption* mOption;
};

} // namespace raft
} // namespace stone
} // namespace apsara

#endif // STONE_CONSENSUS_RAFT_RAFT_REPO_H
