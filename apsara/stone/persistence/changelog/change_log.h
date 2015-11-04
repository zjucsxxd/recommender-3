// Copyright (c) 2013, Alibaba Inc.
// All rights reserved.
//
// Author: DongPing HUANG <dongping.huang@alibaba-inc.com>
// Created: 2013/08/09
// Description:

#ifndef STONE_PERSISTENCE_CHANGELOG_CHANGE_LOG_H
#define STONE_PERSISTENCE_CHANGELOG_CHANGE_LOG_H

#include <string>
#include "stone/base/status.h"
#include "stone/io/file/file_system.h"
#include "stone/memory/uncopyable.h"
#include "stone/persistence/changelog/change_log_reader.h"
#include "stone/persistence/changelog/change_log_types.h"
#include "stone/persistence/changelog/change_log_writer.h"
#include "stone/persistence/changelog/version.h"
#include "stone/string/string_piece.h"
#include "stone/threading/sync/lock.h"

namespace apsara {
namespace stone {

class ChangeLog : private Uncopyable
{
public:
    virtual ~ChangeLog() {}

    virtual Status OpenForRead(ChangeLogReader** reader) = 0;
    virtual Status OpenForWrite(ChangeLogReader** reader,
                                ChangeLogWriter** writer) = 0;
    virtual Status Close() = 0;
};

class ChangeLogImpl : public ChangeLog
{
public:
    ChangeLogImpl(FileSystem* fs, const StringPiece& path,
                  const changelog::Option& option);
    ~ChangeLogImpl();

    Status OpenForRead(ChangeLogReader** reader);
    Status OpenForWrite(ChangeLogReader** reader, ChangeLogWriter** writer);
    Status Close();

private:
    typedef RestrictMutex LockType;
    LockType mLock;

    FileSystem* mFileSystem;
    std::string mPath;
    changelog::Option mOption;
    Version* mVersion;
    ChangeLogReader* mReader;
    ChangeLogWriter* mWriter;
};

} // namespace stone
} // namespace apsara

#endif // STONE_PERSISTENCE_CHANGELOG_CHANGE_LOG_H
