// Copyright (c) 2014, Alibaba Inc.
// All right reserved.
//
// Author: DongPing HUANG <dongping.huang@alibaba-inc.com>
// Created: 2014/01/03
// Description:

#ifndef STONE_CONSENSUS_RAFT_RECOVER_WORKER_H
#define STONE_CONSENSUS_RAFT_RECOVER_WORKER_H

#include <list>
#include "stone/base/status.h"
#include "stone/consensus/raft/raft_fwd.h"
#include "stone/consensus/raft/raft_reader.h"
#include "stone/consensus/raft/raft_server.h"

namespace apsara {
namespace stone {
namespace raft {

class RecoverWorker
{
public:
    RecoverWorker(RaftServer* server,
                  RaftReader* reader,
                  RaftOption* option);
    ~RecoverWorker();

    Status Recover();
    void Cancel();

    raft::LogId GetCommittedId() const;
    raft::LogId GetLastId() const;
    std::list<raft::LogId> GetPending() const;

private:
    bool Canceled() { return mCanceled; }
    Status HandleCkpt();
    Status HandleBlobCkpt();
    Status HandleRecordCkpt();
    Status HandleLog();

private:
    RaftServer* mServer;
    RaftReader* mReader;
    RaftOption* mOption;

    volatile bool mCanceled;

    raft::LogId mCommittedId;
    raft::LogId mLastId;
    std::list<raft::LogId> mPendingId;
};

} // namespace raft
} // namespace stone
} // namespace apsara

#endif // STONE_CONSENSUS_RAFT_RECOVER_WORKER_H
