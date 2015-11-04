// Copyright (c) 2013, Alibaba Inc.
// All right reserved.
//
// Author: ChengYu DONG <chengyu.dong@alibaba-inc.com>
// Created: 2013/05/15
// Description:
// Use Record I/O class to sequential read/write records or logs to a specific
// defined file format. It's the user's responsibility to define record content.
// Record I/O can read records even some parts of file are corrupt. It will seek
// the next valid records automatically without breaking. It also provide some
// useful interfaces to get skipped bytes. Record I/O class won't repair data
// which is corrupt, because it doesn't store external redundancy bytes.

// A logical file format is as following:
// Block Header:
//   Magic         Type    Reserved
//   2 bytes       1 byte  1 byte
//   The variant integer("varint" in short) representing body size.
// Block Body:
//   Header checksum
//   2 bytes
//   record data according to block type:
//     1. Single record:
//       <raw record>
//     2. Consecutive records:
//       N (varint of record size array)
//       [record size array](1..N)
//       <raw record 1>
//       ...
//       <raw record N>
// Block Tailer:
//   Block checksum excluding header checksum and itself.
//   4 bytes
// The Overhead of Record I/O file type is at least 11 bytes. It's better to
// write several small records together to compose a complete block.

#ifndef STONE_IO_RECORDIO_RECORDIO_H
#define STONE_IO_RECORDIO_RECORDIO_H

#include <stdint.h>
#include <stdio.h>
#include <string>

#include "stone/string/string_piece.h"

namespace google {
namespace protobuf {
class Message;
} // namespace protobuf
} // namespace google

namespace apsara {
namespace stone {

// Forward declaration, because we don't publish the detailed structure.
struct BlockInfo;
struct AbstractFile;
class StreamReader;
class FileWriter;

class RecordReader
{
public:
    static const uint32_t kMaxRecordSize = 256 * 1024 * 1024 - 1; // 256M Bytes
    // An option of Record I/O reader is due to whether a user wants to resume
    // last incomplete block. If this option is on, Record I/O won't try to seek
    // for next valid block and just return false to user. It's the user's
    // responsibility to retry reading when a new data is comming.
    enum RecordReaderOptions
    {
        DEFAULT_OPTIONS = 0x0001,
        RESUME_LAST_INCOMPLETE_BLOCK = 0x0002,
        // If having read corrupt data, it will return false when calling Read()
        // function.
        CORRUPTION_SENSITIVE = 0x0004,
    };

    explicit RecordReader(FILE* file,
                          const RecordReaderOptions& options = DEFAULT_OPTIONS);
    explicit RecordReader(std::istream* stream,
                          const RecordReaderOptions& options = DEFAULT_OPTIONS);
    explicit RecordReader(int fd,
                          const RecordReaderOptions& options = DEFAULT_OPTIONS);
    explicit RecordReader(StreamReader * seqReader,
                          const RecordReaderOptions& options = DEFAULT_OPTIONS);

    // Release internal buffer and close the file handle if user doesn't call
    // method Close().
    ~RecordReader();

    // Read next record from the opened file. "data" is a pointer to a pointer
    // of output data address. "size" is also a output parameter representing
    // the data length. The false return value means having read to the end.
    // It's the user's responsibility to copy data if necessary. Record I/O
    // maintains an internal buffer to store data. Buffer data may be transfered
    // to a larger buffer if the current buffer runs out. Old buffer will be
    // deleted then. The false return value means reaching the end of file.
    // Make an option for users who can NOT tolerate data corruption by:
    // Exception: StreamCorruptedException
    // Returns true if there're remaining data to read, false if reaching the
    // end of file.
    bool Read(const char** data, size_t* size);

    // Provide a more convenient interface of string piece.
    bool Read(StringPiece* s);

    // Read protobuf-style message.
    bool ReadMessage(google::protobuf::Message* message);

    // Returns if the corrupt data is found. User who can NOT tolerate any error
    // can call this method.
    bool IsCorruptDetected() const
    {
        return mSkippedBytes > 0;
    }

    // Returns the skipped bytes when detecting corrupt data. It will reset to
    // zero if the next record is fine and completed.
    uint32_t GetSkippedBytes() const
    {
        return mSkippedBytes;
    }

    // Returns the total skipped bytes during the whole lifecycle of this file.
    uint32_t GetTotalSkippedBytes() const
    {
        return mTotalSkippedBytes;
    }

private:
    void Initialize();

    // Load data from file to the internal buffer until it has at least bytes.
    // If having enough data, it won't load from file.
    // Return false when reaching the end of file.
    bool LoadData(uint32_t size);

    // Try reading next valid block. Return false if no valid blocks found OR
    // the end of file reached. If we found corrupt data, the skipped bytes will
    // be accumulated.
    bool ReadBlock();

    // Record I/O reader options.
    // TODO(chengyu.dong): Provide interface to set option at runtime.
    RecordReaderOptions mOptions;
    // Skipped bytes for record-level and file-level.
    uint64_t mSkippedBytes;
    uint64_t mTotalSkippedBytes;
    // Points to head of size array, effective in consecutive-records mode.
    char* mSizeData;
    // Points to user's data.
    char* mRawData;
    // Points to the end of user's data which equals to mRawData + size.
    // size can be read from mSizeData array.
    char* mRawDataEnd;
    // Indicates if having consective zero-byte-record.
    bool mHasConsecutiveZero;
    // Indicates if had read consective zero-byte-record.
    bool mReadingConsecutiveZero;
    // Working info.
    BlockInfo* mBlock;
    // TODO(chengyu.dong): Consider to support Pangu file type.
    AbstractFile* mFile;
};

class RecordWriter
{
public:
    static const uint32_t kMaxRecordSize = RecordReader::kMaxRecordSize;
    enum RecordWriterOptions
    {
        DEFAULT_OPTIONS = 0x0001,
    };

    explicit RecordWriter(FILE* file,
                          const RecordWriterOptions& options = DEFAULT_OPTIONS);
    explicit RecordWriter(std::ostream* stream,
                          const RecordWriterOptions& options = DEFAULT_OPTIONS);
    explicit RecordWriter(int fd,
                          const RecordWriterOptions& options = DEFAULT_OPTIONS);
    explicit RecordWriter(FileWriter* fileWriter,
                          const RecordWriterOptions& options = DEFAULT_OPTIONS);

    ~RecordWriter();

    // Set size of buffer and size of sizebuffer for writer.
    // Default values are 64K and 4K respectively.
    void SetBufferSize(const uint32_t bufSize, const uint32_t sizeBufSize);

    // Sequential write a record to file. It's NOT really written to disk BUT to
    // the internal buffer which can be dumped to disk by OS.
    bool Write(const char* data, size_t size);

    bool Write(const StringPiece& s);

    bool WriteMessage(const google::protobuf::Message& message);

    // User can call this method to flush the internal buffer to disk.
    // TODO(chengyu.dong): Consider to use fsync instead of fflush in
    // implements.
    bool Flush();

private:
    // Write to the internal buffer and update the checksum state.
    bool WriteBlock();

    // Write to file and update the checksum state.
    bool WriteToFile(const char* data, size_t size);

    // Write a 16-bit checksum value without updating checksum state.
    bool WriteChecksum16(uint16_t v);

    // Write a 32-bit checksum value without updating checksum state.
    bool WriteChecksum32(uint32_t v);

    // Write a varint value and update the checksum state.
    bool WriteVarintToFile(uint32_t v);

    // Record I/O writer options.
    RecordWriterOptions mOptions;
    // Working info.
    BlockInfo* mBlock;
    // Abstract file.
    AbstractFile* mFile;
    // Buffer size.
    uint32_t mBufferSize;
    // SizeBuffer size.
    uint32_t mSizeBufferSize;
};

} // namespace stone
} // namespace apsara

#endif // STONE_IO_RECORDIO_RECORDIO_H
