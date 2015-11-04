// Copyright (c) 2013, Alibaba Inc.
// All right reserved.
//
// Author: DongPing HUANG <dongping.huang@alibaba-inc.com>
// Created: 2013/08/09
// Description:

#ifndef STONE_PERSISTENCE_CHANGELOG_JUNCTION_ITERATOR_H
#define STONE_PERSISTENCE_CHANGELOG_JUNCTION_ITERATOR_H

#include <string>
#include <vector>
#include "stone/persistence/changelog/file_list.h"
#include "stone/persistence/changelog/internal_record_iterator.h"
#include "stone/persistence/changelog/record_file_iterator.h"

namespace apsara {
namespace stone {
namespace changelog {

// _JunctionIterator_ provides an iterator interface on edit logs from a group
// of edit log files.
// _JunctionIterator_ get the file list from _FileList_, which is provided in
// order.
// It's not thread safe, it's supposed to be called by a single thread.
class JunctionIterator : public InternalRecordIterator
{
public:
    JunctionIterator(FileSystem* fileSystem, FileList* fileList);
    ~JunctionIterator();

    void Next();
    bool Valid();
    uint64_t SeqNum();
    void Record(Message* message);

private:
    bool MoveToNextFile();

private:
    FileSystem* mFileSystem;
    FileList* mSource;
    std::vector<std::string> mFileList;
    std::string mCurrentFile;
    RecordReader* mReader;
    RecordFileIterator* mCurrent;
};

} // namespace changelog
} // namespace stone
} // namespace apsara

#endif // STONE_PERSISTENCE_CHANGELOG_JUNCTION_ITERATOR_H
