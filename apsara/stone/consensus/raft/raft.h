// Copyright (c) 2013, Alibaba Inc.
// All right reserved.
//
// Author: DongPing HUANG <dongping.huang@alibaba-inc.com>
// Created: 2013/12/03
// Description:

#ifndef STONE_CONSENSUS_RAFT_RAFT_H
#define STONE_CONSENSUS_RAFT_RAFT_H

#include <list>
#include "stone/base/closure.h"
#include "stone/base/status.h"
#include "stone/consensus/raft/raft_fwd.h"
#include "stone/consensus/raft/raft_reader.h"
#include "stone/consensus/raft/raft_role_fwd.h"
#include "stone/consensus/raft/raft_server.h"
#include "stone/consensus/raft/rpc.h"
#include "stone/io/file/local_file_system.h"
#include "stone/threading/runner/task_runner.h"

namespace apsara {
namespace stone {

namespace raft {
class LocalServer;
class RaftImpl;
class RaftRepo;
class RecoverWorker;
} // namespace raft

// Implementation for Raft.
// 1. Provide a publisher/subscriber pattern for client to watch role change.
// 2. User would write operation log to raft. This operation will fail if it is
//    not leader at this moment.
// 3. Recover the checkpoint and operation log written by other previous raft
//    instances.
class Raft : public RaftEventPublisher, public raft::RaftRoleListener
{
public:
    Raft(const RaftOption* option, RaftServer* server);
    ~Raft();

    void Recover();
    void Close();

    // StartMerge and FinishMerge are used to do compaction.
    // These methods are called by user initiatively, when it detected that
    // there are too many operation logs.
    Status StartMerge(LogWriter** writer, RaftReader** reader);
    Status FinishMerge(LogWriter* writer);

    // Register or unregister a subscriber.
    raft::State Subscribe(RaftEventSubscriber* subscriber);
    void Unsubscribe(RaftEventSubscriber* subscriber);

    // write a change log to raft
    Status WriteLog(Message* message, Closure<void, const Status&>* done);

private:
    // interface RaftRoleListener
    void OnRaftRoleChanged(raft::State state);
    void DoRecover();

    void StartService();

private:
    typedef stone::RestrictMutex LockType;
    LockType mLock;
    std::list<RaftEventSubscriber*> mSubscribers;
    RaftOption mOption;
    RaftServer* mRaftServer;
    TaskRunner* mTaskRunner;
    RpcServer* mRpcServer;
    raft::RaftImpl* mRaftImpl;
    raft::RaftRepo* mRepo;
    raft::LocalServer* mLocal;
    raft::State mState;
    LocalFileSystem mFileSystem;
    raft::RecoverWorker* mRecoverWorker;
};

} // namespace stone
} // namespace apsara

#endif // STONE_CONSENSUS_RAFT_RAFT_H
