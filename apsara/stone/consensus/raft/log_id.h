// Copyright (c) 2013, Alibaba Inc.
// All right reserved.
//
// Author: DongPing HUANG <dongping.huang@alibaba-inc.com>
// Created: 2013/12/03
// Description:

#ifndef STONE_CONSENSUS_RAFT_LOG_ID_H
#define STONE_CONSENSUS_RAFT_LOG_ID_H

#include <inttypes.h>
#include "stone/consensus/raft/raft.pb.h"

namespace apsara {
namespace stone {
namespace raft {

inline bool operator<(const LogId& left, const LogId& right)
{
    return left.term() < right.term() ||
        (left.term() == right.term() && left.seq_id() < right.seq_id());
}

inline bool operator==(const LogId& left, const LogId& right)
{
    return left.term() == right.term() && left.seq_id() == right.seq_id();
}

inline bool operator<=(const LogId& left, const LogId& right)
{
    return left < right || left == right;
}

} // namespace raft
} // namespace stone
} // namespace apsara

#endif // STONE_CONSENSUS_RAFT_LOG_ID_H
