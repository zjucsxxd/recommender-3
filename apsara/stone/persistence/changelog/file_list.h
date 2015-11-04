// Copyright (c) 2013, Alibaba Inc.
// All right reserved.
//
// Author: DongPing HUANG <dongping.huang@alibaba-inc.com>
// Created: 2013/08/09
// Description:

#ifndef STONE_PERSISTENCE_CHANGELOG_FILE_LIST_H
#define STONE_PERSISTENCE_CHANGELOG_FILE_LIST_H

#include <string>
#include <vector>
#include "stone/base/status.h"
#include "stone/io/file/file_system.h"
#include "stone/memory/uncopyable.h"
#include "stone/persistence/changelog/version.h"

namespace apsara {
namespace stone {
namespace changelog {

class FileList : private Uncopyable
{
public:
    virtual ~FileList() {}
    virtual Status GetCheckPointFile(std::string* file) = 0;
    virtual Status GetChangeLogFiles(std::vector<std::string>* files) = 0;
};

class FixedFileList : public FileList
{
public:
    FixedFileList(FileSystem* fs, const std::vector<std::string>& files);

    Status GetCheckPointFile(std::string* file);
    Status GetChangeLogFiles(std::vector<std::string>* files);

private:
    FileSystem* mFileSystem;
    std::string mCheckPointFile;
    std::vector<std::string> mChangeLogFiles;
};

class DynamicFileList : public FileList
{
public:
    DynamicFileList(FileSystem* fs, Version* version);
    virtual ~DynamicFileList();

    Status GetCheckPointFile(std::string* file);
    Status GetChangeLogFiles(std::vector<std::string>* files);

private:
    FileSystem* mFileSystem;
    Version* mVersion;
};

} // namespace changelog
} // namespace stone
} // namespace apsara

#endif // STONE_PERSISTENCE_CHANGELOG_FILE_LIST_H
