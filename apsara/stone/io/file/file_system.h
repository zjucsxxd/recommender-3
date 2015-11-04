// Copyright (c) 2013, Alibaba Inc.
// All right reserved.
//
// Author: DongPing HUANG <dongping.huang@alibaba-inc.com>
// Created: 2013/05/15
// Description:

#ifndef STONE_IO_FILE_FILE_SYSTEM_H
#define STONE_IO_FILE_FILE_SYSTEM_H

#include <assert.h>
#include <string>
#include <vector>
#include "stone/base/status.h"
#include "stone/base/uncopyable.h"
#include "stone/io/file/file_writer.h"
#include "stone/io/file/random_reader.h"
#include "stone/io/file/stream_reader.h"
#include "stone/string/string_piece.h"

namespace apsara {
namespace stone {

namespace io {

enum FileType
{
    kUnknown = 0,
    kDirectory = 1,
    kFile = 2,
};

} // namespace io

class FileSystemIterator : private Uncopyable
{
public:
    enum Mode
    {
        kPreOrder = 0,
        kPostOrder = 1,
        kFlatMode = 2,
    };

    virtual ~FileSystemIterator() {}

    virtual void Next() = 0;
    virtual bool Valid() = 0;
    virtual const std::string& Current() const = 0;
    virtual io::FileType Type() const = 0;
};

class FileSystem : private Uncopyable
{
public:
    virtual ~FileSystem() {}

    virtual Status Copy(
        const std::string& source, const std::string& target) = 0;

    virtual Status Delete(const std::string& path, bool recursive) = 0;

    virtual bool Exists(const std::string& path) = 0;

    virtual Status Move(
        const std::string& source, const std::string& target) = 0;

    // create a new file for write
    virtual Status CreateFile(const std::string& path) = 0;

    virtual Status CreateDirectory(const std::string& path) = 0;

    virtual Status List(
        const std::string& path,
        const std::string& pattern,
        std::vector<std::string>* files) = 0;

    virtual bool IsFile(const std::string& path) = 0;

    virtual bool IsDirectory(const std::string& path) = 0;

    virtual char GetPathSeparator() { return '/'; }

    virtual std::string ConcatFilePath(
        const std::string& folder, const std::string& file)
    {
        assert(!folder.empty());
        std::string path(folder);
        char sep = GetPathSeparator();
        if (path[path.size() - 1] != sep) {
            path.push_back(sep);
        }
        path.append(file);

        return path;
    }


    virtual Status GetFileSize(const std::string& file, size_t* size) = 0;

    virtual Status WriteStringToFile(
        const std::string& path, const StringPiece& data) = 0;

    virtual Status ReadFileToString(
        const std::string& path, std::string* data) = 0;

    virtual void CreateStreamReader(
        const std::string& path, StreamReader** reader) = 0;
    virtual void CreateRandomReader(
        const std::string& path, RandomReader** reader) = 0;

    virtual void CreateFileWriter(
        const std::string& path, FileWriter** writer) = 0;

    virtual FileSystemIterator* GetIterator(
        const std::string& root, FileSystemIterator::Mode mode)
    {
        return NULL;
    }
};

} // namespace stone
} // namespace apsara

#endif // STONE_IO_FILE_FILE_SYSTEM_H
