// Copyright (c) 2013, Alibaba Inc.
// All right reserved.
//
// Author: DongPing HUANG <dongping.huang@alibaba-inc.com>
// Created: 2013/12/17
// Description:

#ifndef STONE_CONSENSUS_RAFT_RAFT_SERVER_H
#define STONE_CONSENSUS_RAFT_RAFT_SERVER_H

#include <string>
#include "stone/base/uncopyable.h"

namespace apsara {
namespace stone {

class RaftServer : stone::Uncopyable
{
public:
    virtual ~RaftServer() {}
    virtual void SetCkptBlob(const char* data, size_t size) = 0;
    virtual Message* CreateCkptRecordInstance() = 0;
    virtual void AppendCkptRecord(const Message* record) = 0;
    virtual Message* CreateLogRecordInstance() = 0;
    virtual void AppendLogRecord(const Message* record) = 0;
};

} // namespace stone
} // namespace apsara

#endif // STONE_CONSENSUS_RAFT_RAFT_SERVER_H
