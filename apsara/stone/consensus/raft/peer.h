// Copyright (c) 2013, Alibaba Inc.
// All right reserved.
//
// Author: DongPing HUANG <dongping.huang@alibaba-inc.com>
// Created: 2013/12/03
// Description:

#ifndef STONE_CONSENSUS_RAFT_PEER_H
#define STONE_CONSENSUS_RAFT_PEER_H

#include <list>
#include <string>
#include "stone/consensus/raft/catch_up_worker.h"
#include "stone/consensus/raft/entry.h"
#include "stone/consensus/raft/entry_cache.h"
#include "stone/consensus/raft/log_id.h"
#include "stone/consensus/raft/rpc.h"
#include "stone/persistence/changelog/change_log_reader.h"

namespace apsara {
namespace stone {
namespace raft {

// Peer is a storage for the status and information about a peer.
struct Peer
{
    Peer(int32_t serverId, const std::string& address, RaftService* stub);
    ~Peer();
    void Reset();

    uint32_t serverId;
    std::string address;
    RaftService* stub;
    // whether has sent vote request to this peer
    bool voteRequestSent;
    // whether this peer has voted for me in this term
    bool voted;
    // id of the last log
    LogId lastLog;
    // the latest committed record
    LogId committed;
    // the pending log record not committed yet
    std::list<LogId> pending;
    // when isCaughtUp, write latest op logs, which is in buffer, do not need
    // to read from files; otherwise, need to read from ckpt or oplog file and
    // write to peer.
    bool isCaughtUp;
    CatchUpWorker* catchUpWorker;
    bool obsoletedTermProcessed;
    EntryEx* currentEntry;
    // entryCache is used to catchup tasks.
    EntryCache* entryCache;
    // Beat to the peer if there is not work load send to it before next
    // heart beat time.
    int64_t lastRequestTime;
};

} // namespace raft
} // namespace stone
} // namespace apsara

#endif // STONE_CONSENSUS_RAFT_PEER_H
