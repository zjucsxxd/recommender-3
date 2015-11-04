// Copyright (c) 2013, Alibaba Inc.
// All right reserved.
//
// Author: DongPing HUANG <dongping.huang@alibaba-inc.com>
// Created: 2013/12/23
// Description:

#ifndef STONE_CONSENSUS_RAFT_ENTRY_H
#define STONE_CONSENSUS_RAFT_ENTRY_H

#include <string>
#include <vector>
#include "stone/base/closure.h"
#include "stone/base/ref_counted.h"
#include "stone/base/status.h"
#include "stone/consensus/raft/raft.pb.h"
#include "stone/consensus/raft/rpc.h"

namespace apsara {
namespace stone {
namespace raft {

const int64_t kByteSizeLimitPerEntry = 16 * 1024 * 1024; // 16M

class EntryEx : public RefCountedUnsafe<EntryEx>
{
public:
    // ctor
    EntryEx()
    {
        mByteSize = 0;
    }

    Entry* entry() { return &mEntry; }

    int ByteSize() const { return mByteSize; }

    size_t Size() { return mCallbacks.size(); }

    Closure<void, const Status&>* GetCallback(size_t index)
    {
        return mCallbacks[index];
    }

    Status AppendLog(Message* message, Closure<void, const Status&>* done)
    {
        int64_t bs = message->ByteSize();
        if (this->mByteSize + bs > kByteSizeLimitPerEntry) {
            return Status::kOutOfResource;
        }

        std::string data;
        message->SerializeToString(&data);
        entry()->mutable_log_data()->add_data(data);
        entry()->mutable_log_data()->set_type(raft::DATA);
        this->mCallbacks.push_back(done);
        this->mByteSize += bs;

        return Status::OK();
    }

private:
    friend class RefCountedUnsafe<EntryEx>;
    ~EntryEx() {}

private:
    Entry mEntry;
    // data
    int mByteSize;
    std::vector<Closure<void, const Status&>* > mCallbacks;
};

} // namespace raft
} // namespace stone
} // namespace apsara

#endif // STONE_CONSENSUS_RAFT_ENTRY_H
