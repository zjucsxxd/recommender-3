// Copyright (c) 2013, Alibaba Inc.
// All right reserved.
//
// Author: Ke TANG <ke.tang@alibaba-inc.com>
// Created: 2013/12/20
// Description:

#ifndef STONE_LOGGING_ASYNC_FILE_SINK_H
#define STONE_LOGGING_ASYNC_FILE_SINK_H

#include <stone/logging/file_sink.h>
#include <stone/logging/logging.h>
#include <stone/logging/logging_impl.h>
#include <stone/threading/sync/cond.h>
#include <stone/threading/sync/lock.h>
#include <stone/threading/sync/waitable_event.h>
#include <stone/threading/thread/thread.h>

#include <deque>

namespace apsara {
namespace stone {
namespace logging {

/** Async logging file sink
 * there is an internal thread to write log record to disk file,
 * so that the append log thread will NOT be blocked.
 */
class AsyncFileSink : public ::apsara::stone::logging::Sink
{
public:
    explicit AsyncFileSink(const SinkConfig& config);
    virtual ~AsyncFileSink();
    static AsyncFileSink* Create(const SinkConfig& config);

    /* override */ void AppendLog(LogRecord* record);
    /* override */ void Flush();
    /* override */ void Close();
    /* override */ void Config(const SinkConfig& config);

    void UnderlyingSinkThreadProc();

private:
    void WriteToUnderlyingSink();

    FileSink* mSink; // underlying sink

    typedef ::apsara::stone::SimpleMutex LockType;
    LockType mSinkLock;

    // config parameter
    uint32_t mMaxQueueSize;
    uint64_t mMaxQueueMemSize;
    volatile uint64_t mQueueMemSize;

    struct LogRecordInfo
    {
        LogRecord* record;
        size_t dropped;
    };

    typedef std::deque<LogRecordInfo> RecordQueue;
    RecordQueue mRecordQueue;

    mutable LockType mQueueLock;
    mutable ::apsara::stone::ConditionVariable mQueueCond;

    volatile bool mClosed;
    stone::WaitableEvent mSinkThreadWait;

    int64_t mDroppedRecords;
};

} // namespace logging
} // namespace stone
} // namespace apsara

#endif // STONE_LOGGING_ASYNC_FILE_SINK_H
