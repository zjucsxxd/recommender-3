// Copyright (c) 2013, Alibaba Inc.
// All right reserved.
//
// Author: Ke TANG <ke.tang@alibaba-inc.com>
// Created: 2013/12/20
// Description:

#ifndef STONE_LOGGING_LOGGING_H
#define STONE_LOGGING_LOGGING_H

#include <stone/threading/sync/lock.h>

#include <stone/debug/stack_trace.h>

#include <inttypes.h>
#include <time.h>
#include <unistd.h>
#include <linux/unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>

#include <map>
#include <string>
#include <vector>
#include <tr1/memory>

namespace apsara {
namespace stone {
namespace logging {

extern __thread bool gThreadLogEnabled;

const int kDefaultLogFormattedBufferSize = 4096;

inline void EnableThreadLog(bool enabled = true)
{
    gThreadLogEnabled = enabled;
}

inline bool ThreadLogEnabled()
{
    return gThreadLogEnabled;
}

inline int64_t CONST_NEXT(int64_t init_next, int64_t cur_next)
{
    return init_next;
}

inline int64_t LINEAR_NEXT(int64_t init_next, int64_t cur_next)
{
    return cur_next + init_next;
}

inline int64_t DOUBLE_NEXT(int64_t init_next, int64_t cur_next)
{
    return cur_next * 2;
}

enum LogLevel
{
    LOG_LEVEL_ALL     = 0,      // enable all logs
    LOG_LEVEL_PROFILE = 100,
    LOG_LEVEL_DEBUG   = 200,
    LOG_LEVEL_INFO    = 300,
    LOG_LEVEL_WARNING = 400,
    LOG_LEVEL_ERROR   = 500,
    LOG_LEVEL_FATAL   = 600,
    LOG_LEVEL_NONE    = 10000,  // disable all logs
    LOG_LEVEL_INHERIT = -1      // inherit from the parent
};
    
extern LogLevel LOG_LEVEL_DEFAULT;

class LogInfo
{
public:
    LogInfo(const char* file, int line, LogLevel level, bool with_st);

    void Enable(bool enabled)
    {
        mEnabled = enabled;
    }

    bool Enabled() const
    {
        return mEnabled;
    }

    void EnableStackTrace(bool enabled)
    {
        mStackTraceEnabled = enabled;
    }

    bool StackTraceEnabled() const
    {
        return mStackTraceEnabled;
    }

    LogLevel Level() const
    {
        return mLevel;
    }

    void SetLevel(LogLevel level)
    {
        mLevel = level;
    }
    
    const char* File() const 
    {
        return mFile;
    }

    int Line() const 
    {
        return mLine;
    }

    void Reset()
    {
        mEnabled = true;
        mLevel = mDefaultLevel;
    }

    int64_t Counter() const
    {
        return mCounter;
    }

    int64_t Touch()
    {
        return ++mCounter;
    }

private:
    bool mEnabled;
    bool mStackTraceEnabled;
    LogLevel mDefaultLevel;
    LogLevel mLevel;
    int64_t mCounter;
    const char* mFile;
    int mLine;
};

} // namespace logging
} // namespace stone
} // namespace apsara

/**
 * General Logging
 */
#define STONE_LOG_X_IF(logger, condition, with_st, level, fields...) \
    do { \
        static ::apsara::stone::logging::LogInfo \
                    sInfo(__FILE__, __LINE__, level, with_st); \
        bool logEnabled = condition && \
                          logger->GetMinLevel() <= sInfo.Level() && \
                          sInfo.Enabled() && \
                          ::apsara::stone::logging::ThreadLogEnabled(); \
        if (logEnabled) { \
            std::string stacktrace; \
            if (sInfo.StackTraceEnabled()) { \
                ::apsara::stone::StackTrace st; \
                st.ToString(&stacktrace); \
            } \
            ::apsara::stone::logging::LogBuilder builder; \
            builder.Init( \
                logger->GetPath(), \
                sInfo.Level(), \
                __FILE__, \
                __FUNCTION__, \
                __LINE__, \
                stacktrace \
            )fields; \
            logger->AppendLog(builder, sInfo.Level()); \
        } \
    } while (0)

#define STONE_LOG_X(logger, level, fields...) \
    STONE_LOG_X_IF(logger, true, false, level, fields)

#define STONE_LOG_X_BEGIN(logger, level) \
    do { \
        static ::apsara::stone::logging::LogInfo \
            sInfo(__FILE__, __LINE__, level, false); \
        ::apsara::stone::logging::LogLevel \
            lv_b6d413f4_4944_4ce1_bc3d_5ae51b2cb5f7 = sInfo.Level(); \
        bool enabled_b6d413f4_4944_4ce1_bc3d_5ae51b2cb5f7 = \
            logger->GetMinLevel() <= lv_b6d413f4_4944_4ce1_bc3d_5ae51b2cb5f7 &&\
            sInfo.Enabled() && \
            ::apsara::stone::logging::ThreadLogEnabled(); \
        ::apsara::stone::logging::Logger* \
            tmp_b6d413f4_4944_4ce1_bc3d_5ae51b2cb5f7_logger = logger; \
        ::apsara::stone::logging::LogBuilder \
            builder_b6d413f4_4944_4ce1_bc3d_5ae51b2cb5f7; \
        if (enabled_b6d413f4_4944_4ce1_bc3d_5ae51b2cb5f7) { \
            std::string stacktrace; \
            if (sInfo.StackTraceEnabled()) { \
                ::apsara::stone::StackTrace st; \
                st.ToString(&stacktrace); \
            } \
            builder_b6d413f4_4944_4ce1_bc3d_5ae51b2cb5f7.Init( \
                logger->GetPath(), \
                lv_b6d413f4_4944_4ce1_bc3d_5ae51b2cb5f7, \
                __FILE__, \
                __FUNCTION__, \
                __LINE__, \
                stacktrace \
            ); \
        }

#define STONE_LOG_FIELD(key, value) \
    do { \
        if (enabled_b6d413f4_4944_4ce1_bc3d_5ae51b2cb5f7) { \
            builder_b6d413f4_4944_4ce1_bc3d_5ae51b2cb5f7(key, value); \
        } \
    } \
    while (0)

#define STONE_LOG_X_END \
        if (enabled_b6d413f4_4944_4ce1_bc3d_5ae51b2cb5f7) \
            tmp_b6d413f4_4944_4ce1_bc3d_5ae51b2cb5f7_logger->AppendLog( \
                builder_b6d413f4_4944_4ce1_bc3d_5ae51b2cb5f7, \
                lv_b6d413f4_4944_4ce1_bc3d_5ae51b2cb5f7); \
    } \
    while (0)

#define STONE_LOG_X_SAMPLED(logger, init_next, next_func, level, fields...) \
    do { \
        static ::apsara::stone::logging::LogInfo \
            sInfo(__FILE__, __LINE__, level, false); \
        bool logEnabled = logger->GetMinLevel() <= sInfo.Level() && \
                          sInfo.Enabled() && \
                          ::apsara::stone::logging::ThreadLogEnabled(); \
        if (logEnabled) { \
            std::string stacktrace; \
            if (sInfo.StackTraceEnabled()) { \
                ::apsara::stone::StackTrace st; \
                st.ToString(&stacktrace); \
            } \
            static int64_t sNext = init_next; \
            static ::apsara::stone::SimpleMutex sLock; \
            STONE_SCOPED_LOCK(sLock) { \
                int64_t STONE_LOG_COUNTER;\
                if ((STONE_LOG_COUNTER = sInfo.Touch()) == sNext) { \
                    ::apsara::stone::logging::LogBuilder builder; \
                    builder.Init( \
                        logger->GetPath(), \
                        sInfo.Level(), \
                        __FILE__, \
                        __FUNCTION__, \
                        __LINE__, \
                        stacktrace \
                    )fields; \
                    logger->AppendLog(builder, sInfo.Level()); \
                    sNext = next_func(init_next, STONE_LOG_COUNTER); \
                } \
            }; \
        } \
    } while (0)

// USER INTERFACE SECTION BEGIN

// DEBUG
#define STONE_LOG_DEBUG(logger, fields...) \
    STONE_LOG_X(logger, ::apsara::stone::logging::LOG_LEVEL_DEBUG, fields)

#define STONE_LOG_DEBUG_IF(logger, condition, with_st, fields...) \
    STONE_LOG_X_IF(logger, condition, with_st, \
                   ::apsara::stone::logging::LOG_LEVEL_DEBUG, \
                   fields)

#define STONE_LOG_DEBUG_BEGIN(logger) \
    STONE_LOG_X_BEGIN(logger, ::apsara::stone::logging::LOG_LEVEL_DEBUG)

#define STONE_LOG_DEBUG_END STONE_LOG_X_END

#define STONE_LOG_DEBUG_SAMPLED(logger, next, next_func, fields...) \
    STONE_LOG_X_SAMPLED(logger, next, next_func, \
    ::apsara::stone::logging::LOG_LEVEL_DEBUG, \
    fields)

#define STONE_LOG_DEBUG_F(logger, fields...) \
    STONE_LOG_DEBUG(logger, \
        (::apsara::stone::logging::kDefaultLogFormattedBufferSize, fields))

// INFO
#define STONE_LOG_INFO(logger, fields...) \
    STONE_LOG_X(logger, ::apsara::stone::logging::LOG_LEVEL_INFO, fields)

#define STONE_LOG_INFO_IF(logger, condition, with_st, fields...) \
    STONE_LOG_X_IF(logger, condition, with_st, \
        ::apsara::stone::logging::LOG_LEVEL_INFO, fields)

#define STONE_LOG_INFO_BEGIN(logger) \
    STONE_LOG_X_BEGIN(logger, ::apsara::stone::logging::LOG_LEVEL_INFO)

#define STONE_LOG_INFO_END STONE_LOG_X_END

#define STONE_LOG_INFO_SAMPLED(logger, next, next_func, fields...) \
    STONE_LOG_X_SAMPLED(logger, next, next_func, \
        ::apsara::stone::logging::LOG_LEVEL_INFO, fields)

#define STONE_LOG_INFO_F(logger, fields...) \
    STONE_LOG_INFO(logger, \
        (::apsara::stone::logging::kDefaultLogFormattedBufferSize, fields))

// WARNING
#define STONE_LOG_WARNING(logger, fields...) \
    STONE_LOG_X(logger, ::apsara::stone::logging::LOG_LEVEL_WARNING, fields)

#define STONE_LOG_WARNING_IF(logger, condition, with_st, fields...) \
    STONE_LOG_X_IF(logger, condition, with_st, \
        ::apsara::stone::logging::LOG_LEVEL_WARNING, fields)

#define STONE_LOG_WARNING_BEGIN(logger) \
    STONE_LOG_X_BEGIN(logger, ::apsara::stone::logging::LOG_LEVEL_WARNING)

#define STONE_LOG_WARNING_END STONE_LOG_X_END

#define STONE_LOG_WARNING_SAMPLED(logger, next, next_func, fields...) \
    STONE_LOG_X_SAMPLED(logger, next, next_func, \
        ::apsara::stone::logging::LOG_LEVEL_WARNING, fields)

#define STONE_LOG_WARNING_F(logger, fields...) \
    STONE_LOG_WARNING(logger, \
        (::apsara::stone::logging::kDefaultLogFormattedBufferSize, fields))

// ERROR
#define STONE_LOG_ERROR(logger, fields...) \
    STONE_LOG_X(logger, ::apsara::stone::logging::LOG_LEVEL_ERROR, fields)

#define STONE_LOG_ERROR_IF(logger, condition, with_st, fields...) \
    STONE_LOG_X_IF(logger, condition, with_st, \
        ::apsara::stone::logging::LOG_LEVEL_ERROR, fields)

#define STONE_LOG_ERROR_BEGIN(logger) \
    STONE_LOG_X_BEGIN(logger, ::apsara::stone::logging::LOG_LEVEL_ERROR)

#define STONE_LOG_ERROR_END STONE_LOG_X_END

#define STONE_LOG_ERROR_SAMPLED(logger, next, next_func, fields...) \
    STONE_LOG_X_SAMPLED(logger, next, next_func, \
        ::apsara::stone::logging::LOG_LEVEL_ERROR, fields)

#define STONE_LOG_ERROR_F(logger, fields...) \
    STONE_LOG_ERROR(logger, \
        (::apsara::stone::logging::kDefaultLogFormattedBufferSize, fields))

// FATAL
#define STONE_LOG_FATAL(logger, fields...) \
    STONE_LOG_X(logger, ::apsara::stone::logging::LOG_LEVEL_FATAL, fields)

#define STONE_LOG_FATAL_IF(logger, condition, with_st, fields...) \
    STONE_LOG_X_IF(logger, condition, with_st, \
        ::apsara::stone::logging::LOG_LEVEL_FATAL, fields)

#define STONE_LOG_FATAL_BEGIN(logger) \
    STONE_LOG_X_BEGIN(logger, ::apsara::stone::logging::LOG_LEVEL_FATAL)

#define STONE_LOG_FATAL_END STONE_LOG_X_END

#define STONE_LOG_FATAL_SAMPLED(logger, next, next_func, fields...) \
    STONE_LOG_X_SAMPLED(logger, next, next_func, fields, \
        ::apsara::stone::logging::LOG_LEVEL_FATAL) \

#define STONE_LOG_FATAL_F(logger, fields...) \
    STONE_LOG_FATAL(logger, \
        (::apsara::stone::logging::kDefaultLogFormattedBufferSize, fields))

// STONE_LOG_CALL
#define STONE_LOG_CALL(logger, level, fields...) \
    STONE_LOG_X(logger, level, ("Enter", __FUNCTION__)fields); \
    ::apsara::stone::logging::CallTracer \
        ct_b6d413f4_4944_4ce1_bc3d_5ae51b2cb5f7(logger, level, __FUNCTION__);

#define STONE_LOG_CALL_DEBUG(logger, fields...) \
    STONE_LOG_CALL(logger, ::apsara::stone::logging::LOG_LEVEL_DEBUG, fields)

#define STONE_LOG_CALL_INFO(logger, fields...) \
    STONE_LOG_CALL(logger, ::apsara::stone::logging::LOG_LEVEL_INFO, fields)

#define STONE_LOG_CALL_WARNING(logger, fields...) \
    STONE_LOG_CALL(logger, ::apsara::stone::logging::LOG_LEVEL_WARNING, fields)

#define STONE_LOG_CALL_ERROR(logger, fields...) \
    STONE_LOG_CALL(logger, ::apsara::stone::logging::LOG_LEVEL_ERROR, fields)

#define STONE_LOG_CALL_FATAL(logger, fields...) \
    STONE_LOG_CALL(logger, ::apsara::stone::logging::LOG_LEVEL_FATAL, fields)

// compatible with old apsara logging API
#ifdef ENABLE_LOGGING_COMPATIBILITY

#ifndef APSARA_LOGGER_PREFIX
#define APSARA_LOGGER_PREFIX "AOS:COMMON:LOGGING:"
#endif

#ifdef LOG_X_IF
#undef LOG_X_IF
#endif

#ifdef LOG_X_BEGIN
#undef LOG_X_BEGIN
#endif

#ifdef LOG_FIELD
#undef LOG_FIELD
#endif

#ifdef LOG_X_END
#undef LOG_X_END
#endif

#ifdef LOG_DEBUG
#undef LOG_DEBUG
#endif

#ifdef LOG_DEBUG_IF
#undef LOG_DEBUG_IF
#endif

#ifdef LOG_DEBUG_BEGIN
#undef LOG_DEBUG_BEGIN
#endif

#ifdef LOG_DEBUG_END
#undef LOG_DEBUG_END
#endif

#ifdef LOG_INFO
#undef LOG_INFO
#endif

#ifdef LOG_INFO_IF
#undef LOG_INFO_IF
#endif

#ifdef LOG_INFO_BEGIN
#undef LOG_INFO_BEGIN
#endif

#ifdef LOG_INFO_END
#undef LOG_INFO_END
#endif

#ifdef LOG_WARNING
#undef LOG_WARNING
#endif

#ifdef LOG_WARNING_IF
#undef LOG_WARNING_IF
#endif

#ifdef LOG_WARNING_BEGIN
#undef LOG_WARNING_BEGIN
#endif

#ifdef LOG_WARNING_END
#undef LOG_WARNING_END
#endif

#ifdef LOG_ERROR
#undef LOG_ERROR
#endif

#ifdef LOG_ERROR_IF
#undef LOG_ERROR_IF
#endif

#ifdef LOG_ERROR_BEGIN
#undef LOG_ERROR_BEGIN
#endif

#ifdef LOG_ERROR_END
#undef LOG_ERROR_END
#endif

#ifdef LOG_FATAL
#undef LOG_FATAL
#endif

#ifdef LOG_FATAL_IF
#undef LOG_FATAL_IF
#endif

#ifdef LOG_FATAL_BEGIN
#undef LOG_FATAL_BEGIN
#endif

#ifdef LOG_FATAL_END
#undef LOG_FATAL_END
#endif

// X
#define LOG_X(logger, level, fields...) \
    STONE_LOG_X(logger, true, level, fields)
#define LOG_X_IF(logger, condition, level, fields...) \
    STONE_LOG_X_IF(logger, condition, level, false, fields)
#define LOG_X_BEGIN(logger, level) STONE_LOG_X_BEGIN(logger, level)
#define LOG_FIELD(key, value) STONE_LOG_FIELD(key, value)
#define LOG_X_END STONE_LOG_X_END
#define LOG_X_SAMPLED(logger, next, next_func, level, fields) \
    STONE_LOG_X_SAMPLED(logger, next, next_func, level, fields)

// DEBUG
#define LOG_DEBUG(logger, fields...) STONE_LOG_DEBUG(logger, fields)
#define LOG_DEBUG_IF(logger, condition, fields...) \
    STONE_LOG_DEBUG_IF(logger, condition, false, fields)
#define LOG_DEBUG_BEGIN(logger) STONE_LOG_DEBUG_BEGIN(logger)
#define LOG_DEBUG_END STONE_LOG_DEBUG_END
#define LOG_DEBUG_SAMPLED(logger, next, next_func, fields...) \
    STONE_LOG_DEBUG_SAMPLED(logger, next, next_func, fields)
#define LOG_DEBUG_F(logger, fields...) STONE_LOG_DEBUG_F(logger, fields)

// INFO
#define LOG_INFO(logger, fields...) STONE_LOG_INFO(logger, fields)
#define LOG_INFO_IF(logger, condition, fields...) \
    STONE_LOG_INFO_IF(logger, condition, false, fields)
#define LOG_INFO_BEGIN(logger) STONE_LOG_INFO_BEGIN(logger)
#define LOG_INFO_END STONE_LOG_INFO_END
#define LOG_INFO_SAMPLED(logger, next, next_func, fields...) \
    STONE_LOG_INFO_SAMPLED(logger, next, next_func, fields)
#define LOG_INFO_F(logger, fields...) STONE_LOG_INFO_F(logger, fields)

// WARNING
#define LOG_WARNING(logger, fields...) STONE_LOG_WARNING(logger, fields)
#define LOG_WARNING_IF(logger, condition, fields...) \
    STONE_LOG_WARNING_IF(logger, condition, false, fields)
#define LOG_WARNING_BEGIN(logger) STONE_LOG_WARNING_BEGIN(logger)
#define LOG_WARNING_END STONE_LOG_WARNING_END
#define LOG_WARNING_SAMPLED(logger, next, next_func, fields...) \
    STONE_LOG_WARNING_SAMPLED(logger, next, next_func, fields)
#define LOG_WARNING_F(logger, fields...) STONE_LOG_WARNING_F(logger, fields)

// ERROR
#define LOG_ERROR(logger, fields...) STONE_LOG_ERROR(logger, fields)
#define LOG_ERROR_IF(logger, condition, fields...) \
    STONE_LOG_ERROR_IF(logger, condition, false, fields)
#define LOG_ERROR_BEGIN(logger) STONE_LOG_ERROR_BEGIN(logger)
#define LOG_ERROR_END STONE_LOG_ERROR_END
#define LOG_ERROR_SAMPLED(logger, next, next_func, fields...) \
    STONE_LOG_ERROR_SAMPLED(logger, next, next_func, fields)
#define LOG_ERROR_F(logger, fields...) STONE_LOG_ERROR_F(logger, fields)

// FATAL
#define LOG_FATAL(logger, fields...) STONE_LOG_FATAL(logger, fields)
#define LOG_FATAL_IF(logger, condition, fields...) \
    STONE_LOG_FATAL_IF(logger, condition, false, fields)
#define LOG_FATAL_BEGIN(logger) STONE_LOG_FATAL_BEGIN(logger)
#define LOG_FATAL_END STONE_LOG_FATAL_END
#define LOG_FATAL_SAMPLED(logger, next, next_func, fields...) \
    STONE_LOG_FATAL_SAMPLED(logger, next, next_func, fields)
#define LOG_FATAL_F(logger, fields...) STONE_LOG_FATAL_F(logger, fields)

#ifdef LOG_CALL
#undef LOG_CALL
#endif

#ifdef LOG_CALL_DEBUG
#undef LOG_CALL_DEBUG
#endif

#ifdef LOG_CALL_INFO
#undef LOG_CALL_INFO
#endif

#ifdef LOG_CALL_WARNING
#undef LOG_CALL_WARNING
#endif

#ifdef LOG_CALL_ERROR
#undef LOG_CALL_ERROR
#endif

#ifdef LOG_CALL_FATAL
#undef LOG_CALL_FATAL
#endif

#ifdef LOG_TRACE
#undef LOG_TRACE
#endif

#ifdef LOG_TRACE_IF
#undef LOG_TRACE_IF
#endif

// LOG_CALL
#define LOG_CALL_DEBUG(logger, fields...) STONE_LOG_CALL_DEBUG(logger, fields)

#define LOG_CALL_INFO(logger, fields...) STONE_LOG_CALL_INFO(logger, fields)

#define LOG_CALL_WARNING(logger, fields...) \
    STONE_LOG_CALL_WARNING(logger, fields)

#define LOG_CALL_ERROR(logger, fields...) STONE_LOG_CALL_ERROR(logger, fields)

#define LOG_CALL_FATAL(logger, fields...) STONE_LOG_CALL_FATAL(logger, fields)

namespace apsara {
namespace logging {
std::string GetCurrentTime();
}
}

#define LOG_TRACE_IF(logger, condition, fields, level) \
do { \
    static ::apsara::stone::logging::LogInfo \
    sInfo(__FILE__, __LINE__, level, false); \
    if (condition) { \
        std::string stacktrace; \
        if (sInfo.StackTraceEnabled()) { \
            ::apsara::stone::StackTrace st; \
            st.ToString(&stacktrace); \
        } \
        ::apsara::TraceInfo* ti_b6d413f4_4944_4ce1_bc3d_5ae51b2cb5f7 = \
        reinterpret_cast< ::apsara::TraceInfo*>( \
                pthread_getspecific(::apsara::Tracer::TracerKey) \
                ); \
        if (ti_b6d413f4_4944_4ce1_bc3d_5ae51b2cb5f7 != 0 && \
            ti_b6d413f4_4944_4ce1_bc3d_5ae51b2cb5f7->tid != 0) \
        { \
            ::apsara::stone::logging::LogBuilder builder; \
            builder.Init( \
                apsara::APSARA_TRACER_LOG_PATH, \
                sInfo.Level(), \
                __FILE__, \
                __FUNCTION__, \
                __LINE__, \
                stacktrace) \
            ("__TRACE_ID__", ti_b6d413f4_4944_4ce1_bc3d_5ae51b2cb5f7->tid) \
            ("__TRACE_SID__", ti_b6d413f4_4944_4ce1_bc3d_5ae51b2cb5f7->sid) \
            fields \
            ("__THREAD__", ::apsara::ToString(syscall(SYS_gettid))) \
            ("__TIME__", ::apsara::logging::GetCurrentTime()); \
            ::apsara::stone::logging::GetLogger( \
                    ::apsara::APSARA_TRACER_LOG_PATH\
                    ) \
            ->AppendLog(builder, sInfo.Level()); \
        } \
        if (logger->GetMinLevel() <= level &&\
            ::apsara::stone::logging::ThreadLogEnabled()) \
        { \
            ::apsara::stone::logging::LogBuilder builder; \
            if (ti_b6d413f4_4944_4ce1_bc3d_5ae51b2cb5f7 != 0 && \
                ti_b6d413f4_4944_4ce1_bc3d_5ae51b2cb5f7->tid != 0) \
            { \
                builder.Init( \
                    apsara::APSARA_TRACER_LOG_PATH, \
                    sInfo.Level(), \
                    __FILE__, \
                    __FUNCTION__, \
                    __LINE__, \
                    stacktrace) \
                ("__TRACE_ID__", ti_b6d413f4_4944_4ce1_bc3d_5ae51b2cb5f7->tid) \
                ("__TRACE_SID__", ti_b6d413f4_4944_4ce1_bc3d_5ae51b2cb5f7->sid)\
                fields \
                ("__THREAD__", ::apsara::ToString(syscall(SYS_gettid))) \
                ("__TIME__", ::apsara::logging::GetCurrentTime()); \
                logger->AppendLog(builder, sInfo.Level()); \
            } else { \
                builder.Init( \
                    apsara::APSARA_TRACER_LOG_PATH, \
                    sInfo.Level(), \
                    __FILE__, \
                    __FUNCTION__, \
                    __LINE__, \
                    stacktrace) \
                fields \
                ("__THREAD__", ::apsara::ToString(syscall(SYS_gettid))) \
                ("__TIME__", ::apsara::logging::GetCurrentTime()); \
                logger->AppendLog(builder, sInfo.Level()); \
            } \
        } \
    } \
} \
while (0)

#define LOG_TRACE(logger, fields) \
    LOG_TRACE_IF(logger, true, fields, \
                 ::apsara::stone::logging::LOG_LEVEL_DEBUG)

#endif // ENABLE_LOGGING_COMPATIBILITY

namespace apsara {
namespace stone {
namespace logging {

struct Logger;

void InitLoggingSystem(LogLevel level = LOG_LEVEL_INFO);
void UninitLoggingSystem();

void LoadLogConfig(const std::string& content);
void LoadLogConfigFile(const std::string& filePath = "");

/** Initialize Logger
 *  In each .cpp file that needs logging, create a logger object in the beginning of the file.
 *  @param path input logger path, Example: '/apasra/pangu/master'
 *  @return pointer of the Logger object
 */
Logger* GetLogger(const std::string& path);

void FlushLog();

void EnableLog(const std::string& loc, bool enabled);
void GetAllLogInfo(std::vector<LogInfo*>* infos);

// USER INTERFACE SECTION END

/**This class is only for internal implementation. You should not attempt to use it directly.
 */
class LoggingException : public std::exception
{
};

extern const char* kDefaultFileSinkName;

struct Variant
{
    enum Type
    {
        BOOL,
        INT64,
        DOUBLE,
        STRING
    };

    Type type;

    union
    {
        bool boolValue;
        int64_t int64Value;
        double doubleValue;
    };

    std::string stringValue;
};

struct LogPair : public Variant
{
    std::string key;
};

class LogRecord
{
public:
    LogRecord();
    void AddRef();
    void Release();

    void Init(const std::string& logger, LogLevel level, const char* file,
              const char* function, int line, const std::string& stacktrace);

    void Init(const std::string& logger, LogLevel level, const char* file,
              const char* function, int line, const std::string& stacktrace,
              const timeval& tv);

    uint64_t GetBytes() const;

    const std::string& GetLoggerName() const {
        return mLogger;
    }

    LogLevel GetLogLevel() const {
        return mLevel;
    }

    const char* GetFileName() const {
        return mFile;
    }

    const char* GetFunctionName() const {
        return mFunction;
    }

    int GetLineNum() const {
        return mLine;
    }

    pid_t GetTid() const {
        return mTid;
    }

    timeval GetTime() const {
        return mTime;
    }

    const std::string& GetStackTrace() const {
        return mStackTrace;
    }

    void Add(const LogPair& item) {
        mItems.push_back(item);
    }

    size_t Size() const {
        return mItems.size();
    }

    const LogPair& Item(size_t idx) const {
        return mItems[idx];
    }

private:
    std::string mLogger;
    LogLevel mLevel;
    const char* mFile;
    const char* mFunction;
    int mLine;
    pid_t mTid;
    timeval mTime;
    std::string mStackTrace;

    std::vector<LogPair> mItems;
    int mRefCnt;
};

typedef std::tr1::shared_ptr<LogRecord> LogRecordPtr;

/** Build a log record.
 *
 *  This class is only for internal implementation. You should not
 *  attempt to use it directly.
 */
class LogBuilder
{
    friend struct Logger;

public:
    LogBuilder();
    LogBuilder& Init(const std::string& logger, LogLevel level,
                     const char* file, const char* function, int line,
                     const std::string& stacktrace);

    // Append a key-value pair.

    // null operation
    LogBuilder& operator()();
    // Basic types.
    LogBuilder& operator()(const std::string& key, bool value);
    LogBuilder& operator()(const std::string& key, double value);
    LogBuilder& operator()(const std::string& key, const std::string& value);

    // Conversions
    LogBuilder& operator()(const std::string& key, int32_t value);
    LogBuilder& operator()(const std::string& key, int64_t value);
    LogBuilder& operator()(const std::string& key, const char* value);
    LogBuilder& operator()(const std::string& key, uint32_t value);
    LogBuilder& operator()(const std::string& key, uint64_t value);

    LogBuilder& operator()(const char* key, bool value);
    LogBuilder& operator()(const char* key, double value);
    LogBuilder& operator()(const char* key, const std::string& value);
    LogBuilder& operator()(const char* key, int32_t value);
    LogBuilder& operator()(const char* key, int64_t value);
    LogBuilder& operator()(const char* key, uint32_t value);
    LogBuilder& operator()(const char* key, uint64_t value);
    LogBuilder& operator()(const char* key, const char* value);
    LogBuilder& operator()(size_t size, const char* format, ...);

    LogRecord* GetRecord();

private:
    // Another basic type
    LogBuilder& PrintInt64(const std::string& key, int64_t value);

private:
    LogRecord* mRecord;
};


typedef std::map<std::string, LogLevel> LoggerConfig;

class Logger;
typedef std::tr1::shared_ptr<Logger> LoggerPtr;

/** This class is only for internal implementation. You should not
 *  attempt to use it directly.
 */
class Logger
{
public:
    explicit Logger(const std::string& path);

    void SetDefaultSink();
    void Config(const LoggerConfig& config);
    void AppendLog(const LogBuilder& log, LogLevel level);

    const std::string& GetPath() const {
        return mPath;
    }

    LogLevel GetMinLevel() const {
        return mMinLevel;
    }

    void SetMinLevel(LogLevel level) {
        mMinLevel = level;
    }

    void SetInherited(bool inherited) {
        mInherited = inherited;
    }

    void ClearSinkMap() {
        mSinkMap.clear();
    }

    void AddSink(const std::string& key, LogLevel level) {
        mSinkMap[key] = level;
    }

    bool Inherit(const Logger* logger) {
        if (mInherited) {
            mSinkMap = logger->mSinkMap;
            mMinLevel = logger->mMinLevel;
        }
        return mInherited;
    }

private:
    std::string mPath;
    LogLevel mMinLevel;
    bool mInherited;
    std::map<std::string, LogLevel> mSinkMap;

    typedef ::apsara::stone::RWLock LockType;
    LockType mRWLock;
};

struct CallTracer
{
    CallTracer(Logger* logger, LogLevel level, const char* function);
    ~CallTracer();

    Logger* mLogger;
    LogLevel mLevel;
    const char* mFunction;
};

} // namespace logging
} // namespace stone
} // namespace apsara

#endif // STONE_LOGGING_LOGGING_H
