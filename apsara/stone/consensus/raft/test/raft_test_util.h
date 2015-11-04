// Copyright (c) 2014, Alibaba Inc.
// All right reserved.
//
// Author: DongPing HUANG <dongping.huang@alibaba-inc.com>
// Created: 2014/01/15
// Description:

#ifndef STONE_CONSENSUS_RAFT_TEST_RAFT_TEST_UTIL_H
#define STONE_CONSENSUS_RAFT_TEST_RAFT_TEST_UTIL_H

#include <string>
#include <vector>
#include "gmock/gmock.h"
#include "stone/consensus/raft/raft.pb.h"
#include "stone/consensus/raft/rpc.h"
#include "stone/consensus/raft/test/raft_test.pb.h"
#include "stone/threading/runner/thread_pool.h"

namespace apsara {
namespace stone {
namespace raft {

Record* MakeRecord(int term, int seq, int count, const std::string& pattern)
{
    Record* record = new Record;

    LogId id;
    id.set_term(term);
    id.set_seq_id(seq);
    *record->mutable_id() = id;

    record->set_type(raft::DATA);

    std::string data;
    data.reserve(pattern.size() * count);
    for (int k = 0; k < count; ++k) {
        data.append(pattern);
    }
    record->add_data(data);

    return record;
}

void SetDataRecord(Entry* entry,
                   int term,
                   int seq,
                   const std::vector<std::string>& logs)
{
    Record record;

    LogId id;
    id.set_term(term);
    id.set_seq_id(seq);
    record.mutable_id()->CopyFrom(id);

    record.set_type(raft::DATA);

    for (std::vector<std::string>::const_iterator it = logs.begin();
         it != logs.end();
         ++it) {
        const std::string& str = *it;
        SimpleMessage msg;
        msg.set_msg(str);
        std::string data;
        msg.SerializeToString(&data);
        record.add_data(data);
    }

    entry->mutable_log_data()->CopyFrom(record);
}

void SetCommitRecord(Entry* entry,
                     int term,
                     int seq)
{
    Record record;

    LogId id;
    id.set_term(term);
    id.set_seq_id(seq);
    *record.mutable_id() = id;

    record.set_type(raft::COMMIT);

    entry->mutable_commit()->CopyFrom(record);
}

class RpcCallback
{
public:
    RpcCallback()
      : mThreadPool(1)
    {
        mThreadPool.Startup();
    }
    void Call(RpcController* controller,
              const ::google::protobuf::Message* request,
              ::google::protobuf::Message* response,
              ::google::protobuf::Closure* done)
    {
        mThreadPool.AddTask(done);
    }

private:
    ThreadPool mThreadPool;
};

ACTION_P(CopyFromMessage, msg) { arg0->CopyFrom(msg); }
ACTION(SetControllerAsFail) { arg0->SetFailed("unknown"); }

} // namespace raft
} // namespace stone
} // namespace apsara

#endif // STONE_CONSENSUS_RAFT_TEST_RAFT_TEST_UTIL_H
