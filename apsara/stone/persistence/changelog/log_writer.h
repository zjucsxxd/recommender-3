// Copyright (c) 2013, Alibaba Inc.
// All rights reserved.
//
// Author: DongPing HUANG <dongping.huang@alibaba-inc.com>
// Created: 2013/08/09
// Description:

#ifndef STONE_PERSISTENCE_CHANGELOG_LOG_WRITER_H
#define STONE_PERSISTENCE_CHANGELOG_LOG_WRITER_H

#include <string>
#include "stone/base/status.h"
#include "stone/base/types.h"
#include "stone/io/file/file_system.h"
#include "stone/io/file/file_writer.h"
#include "stone/io/recordio/recordio.h"
#include "stone/memory/uncopyable.h"
#include "stone/persistence/changelog/change_log_types.h"
#include "stone/threading/sync/lock.h"

namespace apsara {
namespace stone {

class LogWriter : private Uncopyable
{
public:
    virtual ~LogWriter() {}

    virtual Status Append(const Message& message) = 0;

    virtual Status WriteBlob(StringPiece data) = 0;
};

class LogWriterImpl : public LogWriter
{
public:
    LogWriterImpl(FileSystem* fs,
                  const std::string& path,
                  changelog::Mode mode);
    ~LogWriterImpl();

    Status Append(const Message& message);

    Status WriteBlob(StringPiece data);

private:
    typedef RestrictMutex LockType;
    LockType mLock;

    FileSystem* mFileSystem;
    std::string mPath;
    changelog::Mode mMode;
    FileWriter* mFileWriter;
    RecordWriter* mRecordWriter;
    uint64_t mSeqId;
};

} // namespace stone
} // namespace apsara

#endif // STONE_PERSISTENCE_CHANGELOG_LOG_WRITER_H
