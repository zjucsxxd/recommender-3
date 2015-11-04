// Copyright (c) 2013, Alibaba Inc.
// All right reserved.
//
// Author: DongPing HUANG <dongping.huang@alibaba-inc.com>
// Created: 2013/05/15
// Description:

#ifndef STONE_IO_FILE_STREAM_READER_H
#define STONE_IO_FILE_STREAM_READER_H

#include "stone/base/status.h"
#include "stone/base/uncopyable.h"

namespace apsara {
namespace stone {

class StreamReader : private Uncopyable
{
public:
    virtual ~StreamReader() {}

    virtual Status Open() = 0;
    virtual Status Read(void* buff, size_t* size) = 0;
    virtual Status Seek(const size_t offset) = 0;
};

} // namespace stone
} // namespace apsara

#endif // STONE_IO_FILE_STREAM_READER_H
