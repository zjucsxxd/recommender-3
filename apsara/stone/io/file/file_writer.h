// Copyright (c) 2013, Alibaba Inc.
// All right reserved.
//
// Author: DongPing HUANG <dongping.huang@alibaba-inc.com>
// Created: 2013/05/15
// Description:

#ifndef STONE_IO_FILE_FILE_WRITER_H
#define STONE_IO_FILE_FILE_WRITER_H

#include "stone/base/status.h"
#include "stone/base/uncopyable.h"
#include "stone/string/string_piece.h"

namespace apsara {
namespace stone {

class FileWriter : private Uncopyable
{
public:
    virtual ~FileWriter() {}

    virtual Status Open() = 0;
    virtual Status Write(const void* data, size_t size) = 0;
    virtual Status Flush() = 0;
    virtual Status GetPosition(size_t * offset) = 0;
};

} // namespace stone
} // namespace apsara

#endif // STONE_IO_FILE_FILE_WRITER_H
