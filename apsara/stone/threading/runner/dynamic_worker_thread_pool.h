// Copyright (c) 2013, Alibaba Inc.
// All right reserved.
//
// Author: DongPing HUANG <dongping.huang@alibaba-inc.com>
// Created: 2013/07/29
// Description:

#ifndef STONE_THREADING_RUNNER_DYNAMIC_WORKER_THREAD_POOL_H
#define STONE_THREADING_RUNNER_DYNAMIC_WORKER_THREAD_POOL_H

#include <string>
#include "stone/threading/atomic/atomic.h"
#include "stone/threading/lockfree/lockfree_queue.h"
#include "stone/threading/lockfree/lockfree_stack.h"
#include "stone/threading/runner/abstract_thread_pool.h"
#include "stone/threading/sync/lock.h"
#include "stone/threading/sync/waitable_event.h"

namespace apsara {
namespace stone {

/**
 * This is a AbstractThreadPool interface implementation which adjusts worker
 * thread dynamically. When add tasks, it may add worker thread if 1) all the
 * worker threads are processing tasks, 2) the current worker threads number
 * is less than thread_num limit of this thread pool. If the thread pool is
 * idle for 79 ms, the worker threads will terminate to save system resources.
 * Current implementation keeps at least one work thread alive.
 */
class DynamicWorkerThreadPool : public AbstractThreadPool
{
public:
    DynamicWorkerThreadPool(
        int threadNum,
        const std::string& name = "threadpool-default");
    ~DynamicWorkerThreadPool();

    // Interface ThreadPoolImpl
    bool Startup();
    bool Shutdown();

    int AddTask(Callback* callback);
    void WaitForIdle();

    int GetQueueLength();
    int GetThreadNum() const;
    int GetBusyThreadNum() const;

protected:
    void SetIdleThresholdInMs(int ms);

private:
    struct Task
    {
        Callback* mCallback;
    };
    struct ThreadInfo
    {
        WaitableEvent mEvent;
    };

    bool IsRunning() const;
    void AddWorkerThread();
    void WorkerRoutine();

    // utility methods for maintaining worker thread
    bool WaitForNotify(ThreadInfo* info);
    void PushIdleThread(ThreadInfo* info);
    bool PopIdleThread(ThreadInfo** info);
    bool RemoveIdleThread(ThreadInfo* info);

    void ExecuteOneTask(Task* task);
    void AtWorkerExit();
    // Get a Task instance from cached free-list, or allocate a new one.
    bool AcquireTask(Task** task);
    // Store the Task instance into cached free-list, or release the memory
    // if there are enough instances cached already.
    void ReleaseTask(Task* task);

private:
    // the time interval before idle thread to exit, in millisecond
    static const int kIdleThresholdInMs;
    // the maximum thread number allowed in a single thread pool
    static const int kMaxThreadNum;
    // the minimum thread number in a single thread pool
    static const int kMinThreadNum;
    // the maximum free tasks in cache list allowed
    static const int kMaxTasksInCache;

    const std::string mName;
    WaitableEvent mEventForAllWorkersExit;

    const int mMaxThreadNum;
    int mThreadNum;
    int mIdleThreadNum;
    int mIdleThresholdInMs;

    LockFreeStack<ThreadInfo*> mIdleThread;

    // This read write lock guard the start and stop of this thread pool.
    typedef SpinRWLock RWLockType;
    RWLockType mRWLock;
    bool mStarted;
    bool mStopped;

    LockFreeQueue<Task*> mTasks;
};

} // namespace stone
} // namespace apsara

#endif // STONE_THREADING_RUNNER_DYNAMIC_WORKER_THREAD_POOL_H
