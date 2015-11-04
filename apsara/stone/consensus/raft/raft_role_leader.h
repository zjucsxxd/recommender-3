// Copyright (c) 2013, Alibaba Inc.
// All right reserved.
//
// Author: DongPing HUANG <dongping.huang@alibaba-inc.com>
// Created: 2013/12/03
// Description:

#ifndef STONE_CONSENSUS_RAFT_RAFT_ROLE_LEADER_H
#define STONE_CONSENSUS_RAFT_RAFT_ROLE_LEADER_H

#include "stone/base/uncopyable.h"
#include "stone/consensus/raft/catch_up_worker.h"
#include "stone/consensus/raft/local_server.h"
#include "stone/consensus/raft/log_id.h"
#include "stone/consensus/raft/peer.h"
#include "stone/consensus/raft/raft.pb.h"
#include "stone/consensus/raft/raft_role.h"
#include "stone/threading/runner/task_runner.h"

namespace apsara {
namespace stone {

class RaftOption;

namespace raft {

struct EntryEx;
class RaftRepo;

class RaftRoleLeader : public RaftRole, public CatchUpListener
{
public:
    RaftRoleLeader(RaftRoleListener* listener,
                   RaftServer* server,
                   RaftRepo* repo,
                   TaskRunner* runner);
    ~RaftRoleLeader();

    // interface RaftRole
    virtual void Start();
    virtual void Stop();

    virtual Status AppendLog(Message* message,
                             Closure<void, const Status&>* done);

    // interface CatchUpListener
    virtual void OnCaughtUp(Peer* peer, bool success);

private:
    // If majority of the servers have accepted the previous entry, start to
    // handle a new one.
    void HandleNewEntry();
    void AppendEntryToPeer(Peer* peer);
    // the following is helper methods for AppendEntry
    void SendRequest(Peer* peer);
    void SendRequestNoLock(Peer* peer);

    // callback when sends AppendEntry to peer
    void OnAppendEntryFinish(EntryEx* entry,
                             Peer* peer,
                             ::google::protobuf::RpcController* controller,
                             const AppendEntry_Request* request,
                             AppendEntry_Response* response);
    void OnAppendEntrySuccess(EntryEx* entry,
                              Peer* peer,
                              const AppendEntry_Request* request,
                              AppendEntry_Response* response);
    void OnAppendEntryFail(EntryEx* entry,
                           Peer* peer,
                           const AppendEntry_Request* request,
                           AppendEntry_Response* response);

    // When write log success, handle callbacks and notify RaftServer.
    void OnWriteLogSuccess(EntryEx* entry);

    void CatchUp(Peer* peer);

    // At lease half of the peers have received the latest entry. Plus the
    // leader, majority of the servers have received it.
    bool HaveMajorityCaughtUp();
    bool IsCurrentEntry(const EntryEx* entry);
    // Whether majority of the servers have accepted CURRENT entry.
    bool HaveMajorityAccepted(const EntryEx* entry);
    // Repair the records belongs to the previous terms. If there are records
    // belongs to previous terms, according to the algorithm, there is at most
    // one record should be committed. In this situation, insert a commit
    // record now.
    void ProcessEntriesFromObsoleteTerm(const LogId& reqId);

    // callback when the timer to keep alive.
    void OnKeepAliveTimeout(uint64_t term,
                            int64_t lastTimeStamp,
                            int64_t taskId);
    void SetKeepAliveTimer();
    void KeepAlive(Peer* peer);

private:
    TaskRunner* mTaskRunner;
};

} // namespace raft
} // namespace stone
} // namespace apsara

#endif // STONE_CONSENSUS_RAFT_RAFT_ROLE_LEADER_H
