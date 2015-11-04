// Copyright (c) 2013, Alibaba Inc.
// All right reserved.
//
// Author: DongPing HUANG <dongping.huang@alibaba-inc.com>
// Created: 2013/12/03
// Description:

#ifndef STONE_CONSENSUS_RAFT_RAFT_FWD_H
#define STONE_CONSENSUS_RAFT_RAFT_FWD_H

#include <inttypes.h>
#include <string>
#include <vector>
#include "stone/consensus/raft/raft.pb.h"
#include "stone/persistence/changelog/change_log.h"

namespace apsara {
namespace stone {

namespace raft {

enum State
{
    kStateFollower = 1,
    kStateCandidate = 2,
    kStateLeader = 3,
};

} // namespace raft

struct RaftOption
{
public:
    struct Server
    {
        int32_t serverId;
        std::string address;
        // Internal use only, init it to NULL when pass to Raft.
        raft::RaftService* stub;
    };

public:
    RaftOption() {}

public:
    int64_t electionTimeout;  // in ms
    std::string ckptFolder;
    std::string oplogFolder;
    changelog::Mode ckptMode;
    Server self;
    std::vector<Server> peers;
};

class RaftEventSubscriber
{
public:
    virtual ~RaftEventSubscriber() {}
    virtual void OnRaftRoleChanged(raft::State state) = 0;
};

class RaftEventPublisher
{
public:
    virtual ~RaftEventPublisher() {}

    // Register or unregister a subscriber.
    virtual raft::State Subscribe(RaftEventSubscriber* subscriber) = 0;
    virtual void Unsubscribe(RaftEventSubscriber* subscriber) = 0;
};

} // namespace stone
} // namespace apsara

#endif // STONE_CONSENSUS_RAFT_RAFT_FWD_H
