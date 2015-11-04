// Copyright (c) 2013, Alibaba Inc.
// All right reserved.
//
// Author: DongPing HUANG <dongping.huang@alibaba-inc.com>
// Created: 2013/05/15
// Description:

#ifndef STONE_IO_FILE_RANDOM_READER_H
#define STONE_IO_FILE_RANDOM_READER_H

#include <inttypes.h>
#include "stone/base/status.h"
#include "stone/base/uncopyable.h"
#include "stone/string/string_piece.h"

namespace apsara {
namespace stone {

class RandomReader : private Uncopyable
{
public:
    virtual ~RandomReader() {}

    virtual Status Open() = 0;
    // Read up to "n" bytes from the file starting at "offset".
    // "scratch[0..n-1]" may be written by this routine.  Sets "*result"
    // to the data that was read (including if fewer than "n" bytes were
    // successfully read).  May set "*result" to point at data in
    // "scratch[0..n-1]", so "scratch[0..n-1]" must be live when "*result"
    // is used.  If an error was encountered, returns a non-OK status.
    virtual Status Read(uint64_t offset, size_t n, StringPiece* result,
                        void* scratch) = 0;
    virtual Status GetSize(size_t* size) = 0;
};

} // namespace stone
} // namespace apsara

#endif // STONE_IO_FILE_RANDOM_READER_H
