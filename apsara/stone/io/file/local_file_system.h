// Copyright (c) 2013, Alibaba Inc.
// All right reserved.
//
// Author: DongPing HUANG <dongping.huang@alibaba-inc.com>
// Created: 2013/05/15
// Description:

#ifndef STONE_IO_FILE_LOCAL_FILE_SYSTEM_H
#define STONE_IO_FILE_LOCAL_FILE_SYSTEM_H

#include <string>
#include <vector>
#include "stone/io/file/file_system.h"

namespace apsara {
namespace stone {

class LocalFileSystem : public FileSystem
{
public:
    Status Copy(const std::string& source, const std::string& target);

    Status Delete(const std::string& path, bool recursive);

    bool Exists(const std::string& path);

    Status Move(
        const std::string& source, const std::string& target);

    // create a new file for write, overriden functions
    Status CreateFile(const std::string& path);
    Status CreateDirectory(const std::string& path);

    Status CreateFile(const std::string& path, bool recursive);
    Status CreateDirectory(const std::string& path, bool recursive);

    Status List(
        const std::string& path,
        const std::string& pattern,
        std::vector<std::string>* files);

    bool IsFile(const std::string& path);

    bool IsDirectory(const std::string& path);

    Status GetFileSize(const std::string& file, size_t* size);

    Status WriteStringToFile(const std::string& path, const StringPiece& data);

    Status ReadFileToString(const std::string& path, std::string* data);

    void CreateStreamReader(
        const std::string& path, StreamReader** reader);
    void CreateRandomReader(
        const std::string& path, RandomReader** reader);
    void CreateFileWriter(const std::string& path, FileWriter** writer);

    FileSystemIterator* GetIterator(const std::string& path,
                                    FileSystemIterator::Mode mode);
};

FileSystemIterator* GetLocalFsIterator(const std::string& path,
                                       FileSystemIterator::Mode mode);

} // namespace stone
} // namespace apsara

#endif // STONE_IO_FILE_LOCAL_FILE_SYSTEM_H
