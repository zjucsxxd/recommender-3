// Copyright (c) 2013, Alibaba Inc.
// All right reserved.
//
// Author: DongPing HUANG <dongping.huang@alibaba-inc.com>
// Created: 2013/05/15
// Description:

#ifndef STONE_SYSTEM_TIME_TIME_STAMP_H
#define STONE_SYSTEM_TIME_TIME_STAMP_H

#include <inttypes.h>

namespace apsara {
namespace stone {

int64_t GetTimeStampInUs();

int64_t GetTimeStampInMs();

} // namespace stone
} // namespace apsara

#endif // STONE_SYSTEM_TIME_TIME_STAMP_H
