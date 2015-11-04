// Copyright (c) 2013, Alibaba Inc.
// All right reserved.
//
// Author: Ke TANG <ke.tang@alibaba-inc.com>
// Created: 2013/12/20
// Description:

#ifndef STONE_LOGGING_FILE_SINK_H
#define STONE_LOGGING_FILE_SINK_H

#include <stone/logging/logging.h>
#include <stone/logging/logging_impl.h>
#include <stone/threading/sync/lock.h>

#include <fstream>
#include <string>
#include <tr1/memory>

namespace apsara {
namespace stone {
namespace logging {

class FileSink : public ::apsara::stone::logging::Sink
{
    friend class FileSinkTest;

public:
    static FileSink* Create(const SinkConfig& config);

    explicit FileSink(const SinkConfig& config);
    virtual ~FileSink();

    /* override */ void AppendLog(LogRecord* record);
    /* override */ void Flush();
    /* override */ void Close();
    /* override */ void Config(const SinkConfig& config);

    void RotateLogFilesOld();
    void RotateLogFiles();
    int DaysFromLastModification(const std::string& filePath);

private:
    std::string GetLogFileName(uint64_t idx);
    std::string ProcessCurrentLogFile();

private:
    std::string mLogFilePath;
    std::string mCurrentLogFilePath;
    std::ofstream mLogFile;

    int64_t mMaxLogFileSize;
    uint64_t mMaxLogFileNum;
    int mMaxDaysFromModify;
    bool mEnableCompression;
    bool mUseOldRotation;
    bool mWithPidSuffix;
    std::string mLogRecordFormat;

    int64_t mLogFileSequence;
    std::tr1::shared_ptr<Formatter> mFormatter;

    typedef ::apsara::stone::SimpleMutex LockType;
    LockType mLock;
};

} // namespace logging
} // namespace stone
} // namespace apsara

#endif // STONE_LOGGING_FILE_SINK_H
