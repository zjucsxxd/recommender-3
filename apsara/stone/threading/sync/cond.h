// Copyright (c) 2013, Alibaba Inc.
// All right reserved.
//
// Author: DongPing HUANG <dongping.huang@alibaba-inc.com>
// Created: 2013/07/29
// Description:

#ifndef STONE_THREADING_SYNC_COND_H
#define STONE_THREADING_SYNC_COND_H

#include <pthread.h>
#include "stone/base/uncopyable.h"
#include "stone/threading/sync/lock.h"

namespace apsara {
namespace stone {

class ConditionVariable : private Uncopyable
{
public:
    explicit ConditionVariable(MutexBase* lock);
    ~ConditionVariable();

    void Wait();
    bool TimedWait(int64_t delayInMs);
    void Signal();
    void Broadcast();

private:
    pthread_mutex_t *mLock;
    pthread_cond_t mCond;
};

} // namespace stone
} // namespace apsara

#endif // STONE_THREADING_SYNC_COND_H
