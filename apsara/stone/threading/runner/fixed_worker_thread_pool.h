// Copyright (c) 2013, Alibaba Inc.
// All right reserved.
//
// Author: DongPing HUANG <dongping.huang@alibaba-inc.com>
// Created: 2013/07/30
// Description:

#ifndef STONE_THREADING_RUNNER_FIXED_WORKER_THREAD_POOL_H
#define STONE_THREADING_RUNNER_FIXED_WORKER_THREAD_POOL_H

#include <string>
#include "stone/threading/atomic/atomic.h"
#include "stone/threading/lockfree/lockfree_queue.h"
#include "stone/threading/runner/abstract_thread_pool.h"
#include "stone/threading/sync/lock.h"
#include "stone/threading/sync/waitable_event.h"

namespace apsara {
namespace stone {

/**
 * This is a ThreadPoolInterface interface implementation with fixed number
 * of worker threads. It's for the cases where the threadpool is always busy.
 * The given number of worker threads will be created when enable the thread
 * pool with Startup(), the threads will not terminate until the threadpool
 * is shutdown, even there isn't any more new tasks coming.
 */
class FixedWorkerThreadPool : public AbstractThreadPool
{
public:
    FixedWorkerThreadPool(int threadNum,
                          const std::string& name = "threadpool-default");
    ~FixedWorkerThreadPool();

    // Interface ThreadPoolImpl
    bool Startup();
    bool Shutdown();

    int AddTask(Callback* callback);
    void WaitForIdle();

    int GetQueueLength();
    int GetThreadNum() const;
    int GetBusyThreadNum() const;

private:
    struct Task
    {
        Callback* mCallback;
    };
    struct ThreadInfo
    {
        WaitableEvent mEvent;
        LockFreeQueue<Task*> mTasks;
        uint32_t mCursorForBorrowTask;
    };

    bool IsRunning() const;
    void WorkerRoutine(int workerIndex);
    bool GetNextTask(int workerIndex, Task** task);
    void WaitForNotify(int workerIndex);
    void ExecuteOneTask(Task* task);
    bool AcquireTask(Task** task);
    void ReleaseTask(Task* task);

private:
    static const int kMaxThreadNum;

    const std::string mName;
    WaitableEvent mEventForAllWorkersExit;

    int mThreadNum;
    int mIdleThreadNum;
    int mStoppedThreadNum;

    int mTryTimesToBorrowTask;

    // This read write lock guard the start and stop of this thread pool.
    typedef SpinRWLock RWLockType;
    RWLockType mRWLock;
    bool mStarted;
    bool mStopped;

    ThreadInfo* mThreadInfo;
    uint64_t mCursorForAddTask;

    // pending tasks number
    int mQueueLength;
};

} // namespace stone
} // namespace apsara

#endif // STONE_THREADING_RUNNER_FIXED_WORKER_THREAD_POOL_H
