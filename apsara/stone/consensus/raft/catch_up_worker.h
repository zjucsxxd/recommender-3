// Copyright (c) 2013, Alibaba Inc.
// All right reserved.
//
// Author: DongPing HUANG <dongping.huang@alibaba-inc.com>
// Created: 2013/12/03
// Description:

#ifndef STONE_CONSENSUS_RAFT_CATCH_UP_WORKER_H
#define STONE_CONSENSUS_RAFT_CATCH_UP_WORKER_H

#include "stone/consensus/raft/raft.pb.h"
#include "stone/consensus/raft/raft_fwd.h"
#include "stone/consensus/raft/rpc.h"
#include "stone/persistence/changelog/change_log_reader.h"
#include "stone/threading/runner/thread_pool.h"

namespace apsara {
namespace stone {
namespace raft {

class LocalServer;
struct Peer;
class RaftRepo;

class CatchUpListener
{
public:
    virtual ~CatchUpListener() {}
    virtual void OnCaughtUp(Peer* peer, bool success) = 0;
};

// When a peer lag behind, us a CatchUpWorker to help to duplicate checkpoint
// and op log records.
class CatchUpWorker
{
public:
    CatchUpWorker(RaftRepo* repo, Peer* peer, CatchUpListener* listener);
    ~CatchUpWorker();

    void Start();
    void Cancel();

private:
    void Routine();
    void QueryLogId();
    void AppendEntryFromCkptFile();
    void AppendEntryFromOpLog();
    bool ReadEntry(raft::Entry* entry);
    bool SendRequest(RpcController* controller,
                     AppendEntry_Request* request,
                     AppendEntry_Response* response);

private:
    RaftRepo* mRepo;
    Peer* mPeer;
    CatchUpListener* mListener;
    LocalServer* mLocal;
    RaftOption* mOption;
    volatile bool mCanceled;
    ThreadPool* mThreadPool;
    RecordIterator* mIterator;
};

} // namespace raft
} // namespace stone
} // namespace apsara

#endif // STONE_CONSENSUS_RAFT_CATCH_UP_WORKER_H
