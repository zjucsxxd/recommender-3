// Copyright (c) 2013, Alibaba Inc.
// All right reserved.
//
// Author: DongPing HUANG <dongping.huang@alibaba-inc.com>
// Created: 2013/12/03
// Description:

#ifndef STONE_CONSENSUS_RAFT_RAFT_IMPL_H
#define STONE_CONSENSUS_RAFT_RAFT_IMPL_H

#include "stone/base/uncopyable.h"
#include "stone/consensus/raft/raft.pb.h"
#include "stone/consensus/raft/raft_role.h"
#include "stone/consensus/raft/raft_server.h"
#include "stone/threading/runner/task_runner.h"

namespace apsara {
namespace stone {

class RaftOption;

namespace raft {

class RaftRepo;
class RecoverWorker;

class RaftImpl : public RaftRoleListener,
                 public RaftService
{
public:
    RaftImpl(RaftServer* server, RaftRepo* repo, TaskRunner* runner);
    ~RaftImpl();

    // interface RaftRoleListener
    virtual void OnRaftRoleChanged(State state);

    // interface RaftService
    void RequestVote(::google::protobuf::RpcController* controller,
                     const raft::RequestVote_Request* request,
                     raft::RequestVote_Response* response,
                     ::google::protobuf::Closure* done);
    void QueryLogId(::google::protobuf::RpcController* controller,
                    const raft::QueryLogId_Request* request,
                    raft::QueryLogId_Response* response,
                    ::google::protobuf::Closure* done);
    void AppendEntry(::google::protobuf::RpcController* controller,
                     const raft::AppendEntry_Request* request,
                     raft::AppendEntry_Response* response,
                     ::google::protobuf::Closure* done);
    void AppendBlob(::google::protobuf::RpcController* controller,
                    const raft::AppendBlob_Request* request,
                    raft::AppendBlob_Response* response,
                    ::google::protobuf::Closure* done);

    // Accept AppendLog request from user.
    Status AppendLog(Message* message, Closure<void, const Status&>* done);

private:
    RaftServer* mServer;
    RaftRepo* mRepo;
    RaftOption* mOption;
    RaftRole* mRoleFollower;
    RaftRole* mRoleCandidate;
    RaftRole* mRoleLeader;
    RaftRole* mRole;
    TaskRunner* mTaskRunner;
};

} // namespace raft
} // namespace stone
} // namespace apsara

#endif // STONE_CONSENSUS_RAFT_RAFT_IMPL_H
