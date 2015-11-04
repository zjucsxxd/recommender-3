// Copyright (c) 2013, Alibaba Inc.
// All right reserved.
//
// Author: DongPing HUANG <dongping.huang@alibaba-inc.com>
// Created: 2013/08/09
// Description:

#ifndef STONE_PERSISTENCE_CHANGELOG_VERSION_H
#define STONE_PERSISTENCE_CHANGELOG_VERSION_H

#include <string>
#include <vector>
#include "stone/base/status.h"
#include "stone/io/file/file_system.h"
#include "stone/memory/ref_counted.h"
#include "stone/persistence/changelog/change_log_types.h"
#include "stone/string/string_piece.h"
#include "stone/threading/sync/lock.h"

namespace apsara {
namespace stone {

class Version : public RefCounted<Version>
{
public:
    Version(FileSystem* fs, const std::string& path);

    // Read stored information back first.
    Status Recover(const changelog::Option& option);
    // _NewLogFile_ Close current log file and create the new one, all the new
    // log record will be written to the new log file.
    Status NewLogFile(std::string* filename);

    // _StartMerge_ starts the merging flow, which is used to merge the latest
    // checkpoint file and editing log files into a new checkpoint file.  A
    // file name is return to the caller.  The caller then is supposed to write
    // the check point content to the file.  The ChangeLog component does not
    // know the merge logic, which is belongs to the caller's business logic.
    // _FinishMerge_ is used to finish the merging flow, including to close the
    // file, to rename it from a temporary name to a final name, and to write a
    // new version file, etc.
    Status StartMerge(std::string* checkpoint, std::string* changelog);
    Status FinishMerge(StringPiece name);

    Status GetFileList(std::vector<std::string>* files);

    // class Version itself doesn't have an efficient way to get the maximum
    // sequence number. On the other hand, every time when recover, a reader
    // iterator is used to traverse all these records, which can get the number
    // easily. _SetMaxSeqNum_ is a little ugly, anyway it's efficient.
    void SetMaxSeqNum(uint64_t seqNum);
    uint64_t NextSeqNum();

private:
    friend class RefCounted<Version>;
    // Call Release instead.
    ~Version();

    Status CheckFolder(const changelog::Option& option);
    Status GetLatestVersionFile(std::string* file);
    Status GetVersionFileList(const std::string& versionFile,
                              std::vector<std::string>* filelist);
    uint64_t NextFileSeqNum();
    // Get file list in the folder, and parse sequence number from the file
    // name, and get the latest one.
    Status GetMaxFileSeqNum(uint32_t* seqNum);
    void ParseFileNameFromFullPath(StringPiece* name);
    void CheckMaxSeqNumFlag();

    Status MoveFile(const std::string& from, const std::string& to);

private:
    enum WorkState
    {
        kInit = 1,
        kRecovering,
        kReady,
        kClosed,
    };

    typedef RestrictMutex LockType;
    LockType mLock;

    FileSystem* mFileSystem;
    // the folder to hold all the data file
    std::string mFilePath;
    std::string mVersionFile;
    std::vector<std::string> mDataFileList;

    // Record sequence number is for the record in the file, file sequence
    // number is for the file themselves.  For the file sequence number, the
    // check point file, the edit log file, and the version file, use the same
    // sequence.
    uint64_t mRecordSeqNum;
    bool mRecordSeqNumSet;
    uint32_t mFileSeqNum;

    WorkState mWorkState;
};

} // namespace stone
} // namespace apsara

#endif // STONE_PERSISTENCE_CHANGELOG_VERSION_H
