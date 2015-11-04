// Copyright (c) 2013, Alibaba Inc.
// All right reserved.
//
// Author: DongPing HUANG <dongping.huang@alibaba-inc.com>
// Created: 2013/08/06
// Description:

#ifndef STONE_THREADING_RUNNER_TASK_RUNNER_DETAIL_H
#define STONE_THREADING_RUNNER_TASK_RUNNER_DETAIL_H

#include "stone/base/closure.h"
#include "stone/containers/intrusive_list.h"
#include "stone/threading/sync/lock.h"

namespace apsara {
namespace stone {
namespace detail {

struct TaskEntry
{
    typedef google::protobuf::Closure Callback;
    typedef AdaptiveMutex LockType;
    DLinkNode mLinkNode;
    LinkNode mNextFreeNode;
    Callback* mCallback;
    LockType mLock;
    int64_t mId;
    bool mInUse;
    bool mInTimeoutQueue;
    bool mCanceled;
    bool mRepeating;
    uint32_t mPendingTasks;

    TaskEntry()
    {
        mCallback = 0;
        Reset();
    }
    ~TaskEntry()
    {
        Reset();
    }
    void Reset();
    void RunOnce();
    bool OnRunOnce();
};

} // namespace detail
} // namespace stone
} // namespace apsara

#endif // STONE_THREADING_RUNNER_TASK_RUNNER_DETAIL_H
