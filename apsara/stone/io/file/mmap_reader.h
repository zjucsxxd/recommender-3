// Copyright (c) 2013, Alibaba Inc.
// All right reserved.
//
// Author: DongPing HUANG <dongping.huang@alibaba-inc.com>
// Created: 2013/05/15
// Description:

#ifndef STONE_IO_FILE_MMAP_READER_H
#define STONE_IO_FILE_MMAP_READER_H

#include <string>
#include "stone/io/file/local_file_system.h"
#include "stone/io/file/random_reader.h"

namespace apsara {
namespace stone {

class MmapReader : public RandomReader
{
public:
    explicit MmapReader(const std::string& filename);
    ~MmapReader();

    virtual Status Open();
    virtual Status Read(uint64_t offset, size_t n,
                        StringPiece* result, void* scratch);
    virtual Status GetSize(size_t* size);

private:
    std::string mFileName;
    Status mStatus;
    void* mStart;
    size_t mLength;
};

} // namespace stone
} // namespace apsara

#endif // STONE_IO_FILE_MMAP_READER_H
