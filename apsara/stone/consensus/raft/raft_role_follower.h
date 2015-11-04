// Copyright (c) 2013, Alibaba Inc.
// All right reserved.
//
// Author: DongPing HUANG <dongping.huang@alibaba-inc.com>
// Created: 2013/12/12
// Description:

#ifndef STONE_CONSENSUS_RAFT_RAFT_ROLE_FOLLOWER_H
#define STONE_CONSENSUS_RAFT_RAFT_ROLE_FOLLOWER_H

#include "stone/base/uncopyable.h"
#include "stone/consensus/raft/local_server.h"
#include "stone/consensus/raft/log_id.h"
#include "stone/consensus/raft/raft.pb.h"
#include "stone/consensus/raft/raft_role.h"
#include "stone/threading/runner/task_runner.h"

namespace apsara {
namespace stone {

class LogWriter;

namespace raft {

class RaftRoleFollower : public RaftRole
{
public:
    RaftRoleFollower(RaftRoleListener* listener,
                     RaftServer* server,
                     RaftRepo* repo,
                     TaskRunner* runner);
    ~RaftRoleFollower();

    // interface RaftRole
    virtual void Start();
    virtual void Stop();

    virtual Status AppendLog(Message* message,
                             Closure<void, const Status&>* done);

private:
    // callback when the timer for beat to peer trigger
    void OnElectionTimeout(int64_t expiredAt, int64_t taskId);
    void SetElectionTimer();

private:
    TaskRunner* mTaskRunner;
    bool mStarted;
};

} // namespace raft
} // namespace stone
} // namespace apsara

#endif // STONE_CONSENSUS_RAFT_RAFT_ROLE_FOLLOWER_H
