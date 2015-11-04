// Copyright (c) 2013, Alibaba Inc.
// All right reserved.
//
// Author: DongPing HUANG <dongping.huang@alibaba-inc.com>
// Created: 2013/07/29
// Description:

#ifndef STONE_THREADING_SYNC_WAITABLE_EVENT_H
#define STONE_THREADING_SYNC_WAITABLE_EVENT_H

#include <inttypes.h>
#include "stone/base/uncopyable.h"

namespace apsara {
namespace stone {

class WaitableEvent : private Uncopyable
{
public:
    explicit WaitableEvent(bool autoReset = true);
    ~WaitableEvent();

    bool Wait(int64_t delayInMs = -1);
    void Reset();
    void Set();

private:
    class Impl;
    Impl* mImpl;
};

} // namespace stone
} // namespace apsara

#endif // STONE_THREADING_SYNC_WAITABLE_EVENT_H
