// Copyright (c) 2013, Alibaba Inc.
// All rights reserved.
//
// Author: DongPing HUANG <dongping.huang@alibaba-inc.com>
// Created: 2013/08/09
// Description:

#ifndef STONE_PERSISTENCE_CHANGELOG_CHANGE_LOG_TYPES_H
#define STONE_PERSISTENCE_CHANGELOG_CHANGE_LOG_TYPES_H

namespace apsara {
namespace stone {
namespace changelog {

enum Mode
{
    kBlobMode = 0,
    kRecordMode = 1,
};

struct Option
{
    Mode mMode;
    bool mCreateIfNotExist;
    int mFlushIntervalInMs;
    int mFlushThreshold;
};

} // namespace changelog
} // namespace stone
} // namespace apsara

#endif // STONE_PERSISTENCE_CHANGELOG_CHANGE_LOG_TYPES_H
