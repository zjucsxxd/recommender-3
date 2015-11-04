// Copyright (c) 2013, Alibaba Inc.
// All right reserved.
//
// Author: DongPing HUANG <dongping.huang@alibaba-inc.com>
// Created: 2013/05/15
// Description:

#ifndef STONE_SYSTEM_TIME_TIME_H
#define STONE_SYSTEM_TIME_TIME_H

#include <stdint.h>

namespace apsara {
namespace stone {

class Time
{
public:
    static const int64_t kMillisecondsPerSecond = 1000;
    static const int64_t kMicrosecondsPerSecond = 1000000;
    static const int64_t kNanosecondsPerSecond = 1000000000;
    static const int64_t kMicrosecondsPerMillisecond = 1000;
    static const int64_t kNanosecondsPerMillisecond = 1000000;
    static const int64_t kNanosecondsPerMicrosecond = 1000;
};

} // namespace stone
} // namespace apsara

#endif // STONE_SYSTEM_TIME_TIME_H
