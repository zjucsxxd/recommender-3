// Copyright (c) 2013, Alibaba Inc.
// All right reserved.
//
// Author: DongPing HUANG <dongping.huang@alibaba-inc.com>
// Created: 2013/05/15
// Description:

#ifndef STONE_IO_FILE_LOCAL_STREAM_READER_H
#define STONE_IO_FILE_LOCAL_STREAM_READER_H

#include <stdio.h>
#include <string>
#include "stone/io/file/stream_reader.h"

namespace apsara {
namespace stone {

class LocalStreamReader : public StreamReader
{
public:
    explicit LocalStreamReader(const std::string& filename);
    ~LocalStreamReader();

    Status Open();
    Status Read(void* buff, size_t* size);
    Status Seek(const size_t offset);

private:
    std::string mFileName;
    FILE* mFile;
    Status mStatus;
};

} // namespace stone
} // namespace apsara

#endif // STONE_IO_FILE_LOCAL_STREAM_READER_H
