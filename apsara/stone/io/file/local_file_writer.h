// Copyright (c) 2013, Alibaba Inc.
// All right reserved.
//
// Author: DongPing HUANG <dongping.huang@alibaba-inc.com>
// Created: 2013/05/15
// Description:

#ifndef STONE_IO_FILE_LOCAL_FILE_WRITER_H
#define STONE_IO_FILE_LOCAL_FILE_WRITER_H

#include <string.h>
#include <string>
#include "stone/base/status.h"
#include "stone/io/file/file_writer.h"

namespace apsara {
namespace stone {

class LocalFileWriter : public FileWriter
{
public:
    explicit LocalFileWriter(const std::string& filename);
    virtual ~LocalFileWriter();

    virtual Status Open();
    virtual Status Write(const void* data, size_t size);
    virtual Status Flush();
    virtual Status GetPosition(size_t* size);

private:
    FILE* mFile;
    Status mStatus;
};

} // namespace stone
} // namespace apsara

#endif // STONE_IO_FILE_LOCAL_FILE_WRITER_H
