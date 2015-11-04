// Copyright (c) 2013, Alibaba Inc.
// All right reserved.
//
// Author: DongPing HUANG <dongping.huang@alibaba-inc.com>
// Created: 2013/12/12
// Description:

#ifndef STONE_CONSENSUS_RAFT_RAFT_ROLE_H
#define STONE_CONSENSUS_RAFT_RAFT_ROLE_H

#include <vector>
#include "stone/base/closure.h"
#include "stone/base/status.h"
#include "stone/consensus/raft/raft.pb.h"
#include "stone/consensus/raft/raft_role_fwd.h"
#include "stone/consensus/raft/raft_server.h"
#include "stone/consensus/raft/rpc.h"
#include "stone/threading/runner/thread_pool.h"

namespace apsara {
namespace stone {
namespace raft {

class RaftRepo;
class Peer;
class LocalServer;

class RaftRole : public RaftService
{
public:
    RaftRole(RaftRoleListener* listener, RaftServer* server, RaftRepo* repo);
    virtual ~RaftRole();

    // For RaftService interface.
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

    // Notify the RaftRole to start/stop, for example, set up the timers, etc.
    virtual void Start() = 0;
    virtual void Stop() = 0;

    virtual Status AppendLog(Message* message,
                             Closure<void, const Status&>* done) = 0;

protected:
    void OnAppendCkptFinish(StringPiece str);
    void OnAppendLogCommitted(LogId id);

protected:
    RaftRoleListener* mListener;
    RaftServer* mServer;
    RaftRepo* mRepo;
    // LocalServer and peers are owned by RaftRepo object.
    LocalServer* mLocal;
    std::vector<Peer*> mPeers;
    // RaftOption is just a pointer, the object is owned by RaftRepo.
    RaftOption* mOption;
    // threadpool used to dispatch log event to RaftServer.
    ThreadPool* mThreadPool;
};

} // namespace raft
} // namespace stone
} // namespace apsara

#endif // STONE_CONSENSUS_RAFT_RAFT_ROLE_H
