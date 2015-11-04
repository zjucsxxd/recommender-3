// Copyright (c) 2013, Alibaba Inc.
// All right reserved.
//
// Author: DongPing HUANG <dongping.huang@alibaba-inc.com>
// Created: 2013/08/09
// Description:

#ifndef STONE_PERSISTENCE_CHANGELOG_CHANGE_LOG_WRITER_H
#define STONE_PERSISTENCE_CHANGELOG_CHANGE_LOG_WRITER_H

#include <list>
#include <string>
#include "stone/base/closure.h"
#include "stone/base/status.h"
#include "stone/base/types.h"
#include "stone/io/file/file_system.h"
#include "stone/memory/ref_counted.h"
#include "stone/persistence/changelog/change_log_reader.h"
#include "stone/persistence/changelog/change_log_types.h"
#include "stone/persistence/changelog/log_writer.h"
#include "stone/persistence/changelog/version.h"
#include "stone/threading/sync/cond.h"
#include "stone/threading/sync/waitable_event.h"

namespace apsara {
namespace stone {

class ChangeLogWriter : public RefCounted<ChangeLogWriter>
{
public:
    typedef Closure<void, const Status&> Callback;

    virtual Status NewLogFile() = 0;
    virtual Status Append(const Message& message, Callback* done = NULL) = 0;

    virtual Status StartMerge(LogWriter** writer,
                              ChangeLogReader** reader = NULL) = 0;
    virtual Status FinishMerge(LogWriter* writer) = 0;

protected:
    friend class RefCounted<ChangeLogWriter>;
    // use Release instead
    virtual ~ChangeLogWriter() {}
};

class ChangeLogWriterImpl : public ChangeLogWriter
{
public:
    ChangeLogWriterImpl(FileSystem* fs,
                        Version* version,
                        const changelog::Option& option);

    Status NewLogFile();
    Status Append(const Message& message, Callback* done = NULL);

    Status StartMerge(LogWriter** writer, ChangeLogReader** reader = NULL);
    Status FinishMerge(LogWriter* writer);

private:
    ~ChangeLogWriterImpl();
    Status NewLogFileNoLock();
    Status SwitchRecordFile(const std::string& filename);
    void OnSyncAppendFinished(ConditionVariable* cond,
                              Status* pStatus,
                              const Status& status);

    Status AppendNoLock(const Message& message, Callback* done);
    void Routine();

private:
    typedef RestrictMutex LockType;
    LockType mLock;

    FileSystem* mFileSystem;

    changelog::Option mOption;
    Version* mVersion;
    // the writer for append edit log
    FileWriter* mFileWriter;
    RecordWriter* mRecordWriter;
    // the writer for merge process
    LogWriter* mMergeWriter;
    std::string mMergeFile;

    // these tasks have not beed flushed yet
    std::list<Callback*> mTasksPending;
    int mTasksPendingCount;
    // the backgroup thread is flush these tasks
    std::list<Callback*> mTasksFlushing;

    bool mStopped;
    WaitableEvent mEventForWorker;
    WaitableEvent mEventForExit;
};

} // namespace stone
} // namespace apsara

#endif // STONE_PERSISTENCE_CHANGELOG_CHANGE_LOG_WRITER_H
