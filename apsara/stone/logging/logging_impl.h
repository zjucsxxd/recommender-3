// Copyright (c) 2013, Alibaba Inc.
// All right reserved.
//
// Author: Ke TANG <ke.tang@alibaba-inc.com>
// Created: 2013/12/20
// Description:

#ifndef STONE_LOGGING_LOGGING_IMPL_H
#define STONE_LOGGING_LOGGING_IMPL_H

#include <stone/logging/logging.h>
#include <stone/threading/sync/lock.h>

#include <signal.h>

#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>
#include <tr1/functional>
#include <tr1/memory>

namespace apsara {
namespace stone {
namespace logging {

class SinkConfig {
public:
    template<typename T>
    bool Get(const std::string& key, T* val) const
    {
        typeof(mConfig.end()) it = mConfig.find(key);
        if (it == mConfig.end()) {
            return false;
        }

        std::stringstream ss(it->second);
        ss >> *val;
        return true;
    }

    bool Get(const std::string& key, std::string* val) const
    {
        typeof(mConfig.end()) it = mConfig.find(key);
        if (it == mConfig.end()) {
            return false;
        }

        *val = it->second;
        return true;
    }

    bool Get(const std::string& key, bool* flag) const
    {
        std::string val;
        if (!Get(key, &val)) {
            return false;
        }

        *flag = strcasecmp(val.c_str(), "true") == 0 ||
                strcasecmp(val.c_str(), "yes") == 0;
        return true;
    }

    template<typename T>
    void Get(const std::string& key, T* val, T defVal) const
    {
        if (!Get(key, val)) {
            *val = defVal;
        }
    }

    template<typename T1, typename T2>
    void Get(const std::string& key, T1* val, T2 defVal) const
    {
        if (!Get(key, val)) {
            *val = static_cast<T1>(defVal);
        }
    }

    bool Add(const std::string& key, const std::string& val)
    {
        return mConfig.insert(make_pair(key, val)).second;
    }

private:
    std::map<std::string, std::string> mConfig;
};

class Sink {
public:
    virtual ~Sink() {}
    virtual void Config(const SinkConfig& config) = 0;
    virtual void AppendLog(LogRecord* record) = 0;
    virtual void Flush() = 0;
    virtual void Close() = 0;
};

typedef std::tr1::shared_ptr<Sink> SinkPtr;

class Formatter
{
public:
    virtual ~Formatter() {}
    virtual void Format(std::ostream* output, const LogRecord& record) = 0;
};

class DefaultFormatter : public Formatter
{
public:
    virtual void Format(std::ostream* output, const LogRecord& record);
};

typedef void (*OutputProc)(std::ostream* output,
                           const LogRecord& record,
                           void* tag);
class OutputProcInfo;

class UserDefinedFormatter : public Formatter
{
public:
    explicit UserDefinedFormatter(const std::string& pattern);
    virtual void Format(std::ostream* output, const LogRecord& record);

private:
    struct OutputPart
    {
        OutputProc output;
        Variant item;
    };

    void ParseOutputPattern(const std::string& pattern,
                            const OutputProcInfo* outputProcs,
                            std::vector<OutputPart>* outputParts);

private:
    std::string mFormatter;

    std::vector<OutputPart> mBasicOutputParts;
    std::vector<OutputPart> mUserOutputParts;
};

typedef std::tr1::function<Sink*(const SinkConfig&)> SinkFactory;

class LoggingSystem
{
public:
    static void Init();
    static void Uninit();
    static void RegisterSinkFactory(std::string type,
                                    const SinkFactory& factory);
    static void LoadConfig(const std::string& config);
    static void FlushAllSinks();
    static void CloseAllSinks();
    static SinkPtr GetSink(const std::string& type);
    static LoggerPtr GetLogger(const std::string& path);
    static void RegisterLogInfo(LogInfo* info);
    static LogInfo* GetLogInfo(const std::string& loc);
    static void GetAllLogInfo(std::vector<LogInfo*>* infos);
    static void ConfigLogger(Logger* logger, const LoggerConfig& config);
    static void ConfigLogger(const std::string& name,
                             const LoggerConfig& config);
    static void ConfigSink(const std::string& name, const SinkConfig& config);
};

class LoggingSystemImpl
{
public:
    LoggingSystemImpl();

    static LoggingSystemImpl& GetInstance();

    void Init();
    void Uninit();
    void RegisterSinkFactory(std::string type,
                             const SinkFactory& factory);
    void LoadConfig(const std::string& config);
    void FlushAllSinks();
    void CloseAllSinks();

    SinkPtr GetSink(const std::string& type);
    LoggerPtr GetLogger(const std::string& path);
    void RegisterLogInfo(LogInfo* info);
    LogInfo* GetLogInfo(const std::string& loc);
    void GetAllLogInfo(std::vector<LogInfo*>* infos);
    void ConfigLogger(Logger* logger, const LoggerConfig& config);
    void ConfigLogger(const std::string& name, const LoggerConfig& config);
    void ConfigSink(const std::string& name, const SinkConfig& config);
    int GZipFile(const std::string& fname);

    struct sigaction act, oact;

private:
    void DoInit();
    void ConfigLoggerNoLock(Logger* logger, const LoggerConfig& config);
    void ConfigSubLoggers(Logger* logger);
    void ConfigNonInheritedLogger(Logger* logger, const LoggerConfig& config);
    void ConfigInheritedLogger(Logger* logger);

private:
    typedef ::apsara::stone::SimpleMutex LockType;
    LockType mLock;
    bool mInitialized;

    std::map<std::string, LoggerPtr> mLoggers;
    std::map<std::string, SinkPtr> mSinks;
    std::map<std::string, LogInfo*> mLogInfos;
    std::map<std::string, SinkFactory> mSinkFactories;

    typedef void* (*GzOpen)(const char* path, const char* mode);
    typedef int   (*GzWrite)(void* file, void* buf, unsigned int len);
    typedef int   (*GzClose)(void* file);

    GzOpen mGzOpen;
    GzWrite mGzWrite;
    GzClose mGzClose;
};

// utility functions
std::string GetProgramPath();
std::string GetProgramDirectory();
std::string GetProgramName();

LogLevel StringToLogLevel(const std::string& s);

} // namespace logging
} // namespace stone
} // namespace apsara

#endif // STONE_LOGGING_LOGGING_IMPL_H
