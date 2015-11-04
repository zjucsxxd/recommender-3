// Copyright (c) 2014, Alibaba Inc.
// All right reserved.
//
// Author: DongPing HUANG <dongping.huang@alibaba-inc.com>
// Created: 2014/01/08
// Description:

#ifndef STONE_CONSENSUS_RAFT_BLOB_H
#define STONE_CONSENSUS_RAFT_BLOB_H

#include <inttypes.h>
#include "stone/base/scoped_ptr.h"
#include "stone/persistence/changelog/log_writer.h"
#include "stone/string/string_piece.h"

namespace apsara {
namespace stone {
namespace raft {

class Blob
{
public:
    Blob(LogWriter* writer, uint64_t length);
    ~Blob();

    bool Write(const StringPiece& str, uint64_t pos);
    bool Finished();
    bool GetBuffer(StringPiece* str);

private:
    LogWriter* mWriter;
    scoped_array<char> mBuffer;
    uint64_t mLength;
    uint64_t mPos;
};

} // namespace raft
} // namespace stone
} // namespace apsara

#endif // STONE_CONSENSUS_RAFT_BLOB_H
