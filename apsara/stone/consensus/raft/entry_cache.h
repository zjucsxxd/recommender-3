// Copyright (c) 2013, Alibaba Inc.
// All right reserved.
//
// Author: DongPing HUANG <dongping.huang@alibaba-inc.com>
// Created: 2013/12/03
// Description:

#ifndef STONE_CONSENSUS_RAFT_ENTRY_CACHE_H
#define STONE_CONSENSUS_RAFT_ENTRY_CACHE_H

#include <inttypes.h>
#include <list>
#include "stone/consensus/raft/entry.h"

namespace apsara {
namespace stone {
namespace raft {

struct EntryEx;
const int64_t kDefaultCacheLimit = 128 * 1024 * 1024;

// A list used to manage the entries pending for a peer.
class EntryCache
{
public:
    explicit EntryCache(int64_t byteSizeLimit = kDefaultCacheLimit);
    ~EntryCache();

    void Clear();
    bool Empty();
    raft::EntryEx* Front();
    void PopFront();
    void PushBack(EntryEx* entry, bool* overflow = NULL);
    // Get the serialized size of all the entry.
    int64_t ByteSize();
    size_t Size();

private:
    struct Item
    {
        EntryEx* entry;
        size_t size;
    };
    std::list<Item> mEntries;
    // The serialized size of the message.
    const int64_t mByteSizeLimit;
    int64_t mByteSize;
};

} // namespace raft
} // namespace stone
} // namespace apsara

#endif // STONE_CONSENSUS_RAFT_ENTRY_CACHE_H
