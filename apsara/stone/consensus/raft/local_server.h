// Copyright (c) 2013, Alibaba Inc.
// All right reserved.
//
// Author: DongPing HUANG <dongping.huang@alibaba-inc.com>
// Created: 2013/12/03
// Description:

#ifndef STONE_CONSENSUS_RAFT_LOCAL_SERVER_H
#define STONE_CONSENSUS_RAFT_LOCAL_SERVER_H

#include <list>
#include <string>
#include "stone/consensus/raft/blob.h"
#include "stone/consensus/raft/entry.h"
#include "stone/consensus/raft/entry_cache.h"
#include "stone/consensus/raft/log_id.h"
#include "stone/consensus/raft/raft.pb.h"
#include "stone/consensus/raft/raft_fwd.h"
#include "stone/consensus/raft/rpc.h"

namespace apsara {
namespace stone {
namespace raft {

struct LocalServer
{
public:
    LocalServer(uint32_t serverId,
                const std::string& address,
                ChangeLog* logger);
    ~LocalServer();

    void Reset();

    Status Recover();
    Status AppendLog(Message* message, Closure<void, const Status&>* done);

private:
    void CancelEntryEx(EntryEx* entry, const Status& s);

public:
    State state;
    uint32_t serverId;
    uint32_t leaderId;
    std::string address;
    uint64_t votedFor;
    uint64_t term;
    LogId lastLog;
    LogId committed;
    std::list<LogId> pending;
    //
    EntryCache* entryCache;
    EntryEx* currentEntry;
    EntryEx* nextEntry;
    bool obsoletedTermProcessed;
    // Only vote once before electTimeout.
    int64_t electTimeout;
    ChangeLog* changeLog;
    ChangeLogWriter* logWriter;
    ChangeLogReader* logReader;
    Blob* blob;
};

} // namespace raft
} // namespace stone
} // namespace apsara

#endif // STONE_CONSENSUS_RAFT_LOCAL_SERVER_H
