// Copyright (c) 2013, Alibaba Inc.
// All right reserved.
//
// Author: DongPing HUANG <dongping.huang@alibaba-inc.com>
// Created: 2013/12/03
// Description:

#ifndef STONE_CONSENSUS_RAFT_RAFT_READER_H
#define STONE_CONSENSUS_RAFT_RAFT_READER_H

#include <list>
#include "stone/base/ref_counted.h"
#include "stone/consensus/raft/log_id.h"
#include "stone/consensus/raft/raft.pb.h"
#include "stone/persistence/changelog/change_log.h"

namespace apsara {
namespace stone {

class RaftEntryIterator : public RecordIterator
{
public:
    explicit RaftEntryIterator(RecordIterator* iterator);
    ~RaftEntryIterator();
    virtual void Next();
    virtual bool Valid();
    virtual void Record(Message* message);

    raft::LogId GetCommittedId() const;
    raft::LogId GetLastId() const;
    std::list<raft::LogId> GetPending() const;

private:
    RecordIterator* mIterator;
    std::list<raft::Entry*> mPending;
    raft::LogId mCommitted;
    raft::LogId mLastId;
    raft::Entry* mCurrent;
};

class RaftRecordIterator : public RecordIterator
{
public:
    explicit RaftRecordIterator(RaftEntryIterator* iterator);
    ~RaftRecordIterator();
    virtual void Next();
    virtual bool Valid();
    virtual void Record(Message* message);

    virtual raft::LogId GetCommittedId() const;
    virtual raft::LogId GetLastId() const;
    virtual std::list<raft::LogId> GetPending() const;

private:
    RaftEntryIterator* mIterator;
    raft::Record* mRecord;
    int64_t mSize;
    int64_t mIndex;
};

class RaftReader : public RefCountedUnsafe<RaftReader>
{
public:
    // Methods for read checkpoint.
    virtual Status GetCkptBlobReader(RandomReader** reader) = 0;
    virtual Status GetCkptRecordIterator(RecordIterator** iterator) = 0;
    // Method for read op log.
    virtual RaftRecordIterator* GetLogIterator() = 0;

protected:
    friend class RefCountedUnsafe<RaftReader>;
    virtual ~RaftReader() {}
};

class RaftReaderImpl : public RaftReader
{
public:
    explicit RaftReaderImpl(ChangeLogReader* reader);

    // Methods for read checkpoint.
    virtual Status GetCkptBlobReader(RandomReader** reader);
    virtual Status GetCkptRecordIterator(RecordIterator** iterator);
    // Method for read op log.
    virtual RaftRecordIterator* GetLogIterator();

protected:
    virtual ~RaftReaderImpl();

protected:
    ChangeLogReader* mLogReader;
};

} // namespace stone
} // namespace apsara

#endif // STONE_CONSENSUS_RAFT_RAFT_READER_H
