// Copyright (c) 2013, Alibaba Inc.
// All right reserved.
//
// Author: DongPing HUANG <dongping.huang@alibaba-inc.com>
// Created: 2013/12/12
// Description:

#ifndef STONE_CONSENSUS_RAFT_RAFT_ROLE_CANDIDATE_H
#define STONE_CONSENSUS_RAFT_RAFT_ROLE_CANDIDATE_H

#include "stone/base/uncopyable.h"
#include "stone/consensus/raft/local_server.h"
#include "stone/consensus/raft/log_id.h"
#include "stone/consensus/raft/peer.h"
#include "stone/consensus/raft/raft.pb.h"
#include "stone/consensus/raft/raft_role.h"
#include "stone/threading/runner/task_runner.h"

namespace apsara {
namespace stone {
namespace raft {

class Peer;

class RaftRoleCandidate : public RaftRole
{
public:
    RaftRoleCandidate(RaftRoleListener* listener,
                      RaftServer* server,
                      RaftRepo* repo,
                      TaskRunner* runner);
    ~RaftRoleCandidate();

    // interface RaftRole
    virtual void Start();
    virtual void Stop();

    virtual Status AppendLog(Message* message,
                             Closure<void, const Status&>* done);

private:
    void SetElectionTimer();
    void OnRequestVoteTimeout(uint64_t term, int64_t expiredAt);
    void SendVoteRequest();
    void SendVoteRequest(Peer* peer);
    void OnRequestVoteFinish(
        ::google::protobuf::RpcController* controller,
        const raft::RequestVote_Request* request,
        raft::RequestVote_Response* response,
        Peer* peer);

private:
    TaskRunner* mTaskRunner;
    bool mStarted;
    uint32_t mGrantedCount;
    uint32_t mRejectedCount;
};

} // namespace raft
} // namespace stone
} // namespace apsara

#endif // STONE_CONSENSUS_RAFT_RAFT_ROLE_CANDIDATE_H
