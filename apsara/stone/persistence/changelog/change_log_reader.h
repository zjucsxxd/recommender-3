// Copyright (c) 2013, Alibaba Inc.
// All right reserved.
//
// Author: DongPing HUANG <dongping.huang@alibaba-inc.com>
// Created: 2013/08/09
// Description:

#ifndef STONE_PERSISTENCE_CHANGELOG_CHANGE_LOG_READER_H
#define STONE_PERSISTENCE_CHANGELOG_CHANGE_LOG_READER_H

#include <string>
#include "stone/base/types.h"
#include "stone/io/file/file_system.h"
#include "stone/io/file/random_reader.h"
#include "stone/memory/ref_counted.h"
#include "stone/persistence/changelog/change_log_types.h"
#include "stone/persistence/changelog/file_list.h"
#include "stone/persistence/changelog/version.h"

namespace apsara {
namespace stone {

class RecordIterator : private Uncopyable
{
public:
    virtual ~RecordIterator() {}

    virtual void Next() = 0;
    virtual bool Valid() = 0;
    virtual void Record(Message* message) = 0;
};

class ChangeLogReader : public RefCounted<ChangeLogReader>
{
public:
    virtual Status GetBlobReader(RandomReader** reader) = 0;
    virtual Status GetCheckPointIterator(RecordIterator** iterator) = 0;

    virtual RecordIterator* GetChangeLogIterator() = 0;

protected:
    friend class RefCounted<ChangeLogReader>;
    // Don't call dtor directly, use Release instead.
    virtual ~ChangeLogReader() {}
};

class ChangeLogReaderImpl : public ChangeLogReader
{
public:
    ChangeLogReaderImpl(FileSystem* fs,
                        Version* version,
                        changelog::FileList* fileList,
                        const changelog::Option& option);

    Status GetBlobReader(RandomReader** reader);
    Status GetCheckPointIterator(RecordIterator** iterator);

    RecordIterator* GetChangeLogIterator();

private:
    ~ChangeLogReaderImpl();
    bool GetCheckPointFileName(std::string* name);

private:
    FileSystem* mFileSystem;
    Version* mVersion;
    changelog::FileList* mFileList;
    changelog::Option mOption;
};

} // namespace stone
} // namespace apsara

#endif // STONE_PERSISTENCE_CHANGELOG_CHANGE_LOG_READER_H

