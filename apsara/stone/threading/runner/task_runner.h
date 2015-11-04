// Copyright (c) 2013, Alibaba Inc.
// All right reserved.
//
// Author: DongPing HUANG <dongping.huang@alibaba-inc.com>
// Created: 2013/08/06
// Description:

#ifndef STONE_THREADING_RUNNER_TASK_RUNNER_H
#define STONE_THREADING_RUNNER_TASK_RUNNER_H

#include <list>
#include "stone/base/closure.h"
#include "stone/base/uncopyable.h"
#include "stone/containers/intrusive_list.h"
#include "stone/threading/lockfree/free_list.h"
#include "stone/threading/runner/abstract_task_runner.h"
#include "stone/threading/runner/task_runner_detail.h"
#include "stone/threading/runner/thread_pool.h"
#include "stone/threading/sync/lock.h"

namespace apsara {
namespace stone {

class TimeoutQueue;

// TaskRunner is a pump for asynchronized tasks. It supports immediate
// task, delayed task, and repeating task.
// Because the task is executed in asynchronized way, DO NOT delete the
// objects refered by the callback before it finishes. You may need a way
// to record the tasks added into the task runner, and update the records
// in the callback.
class TaskRunner : public AbstractTaskRunner
{
public:
    TaskRunner(int thread_num_limit = 2, int timeout_queue_limit = 65536);
    ~TaskRunner();

    void Startup();
    void Shutdown();

    // Get the thread pool object in the task runner
    // DO NOT use it any more after shutdown the task runner.
    ThreadPool* GetThreadPool();

    // Get the timeout queue object in the task runner
    // DO NOT use it any more after shutdown the task runner.
    TimeoutQueue* GetTimeoutQueue();

    // Add immediate task, which is not cancelable. The immediate tasks
    // will be executed unless the task run is shutdown before the task
    // executed.
    bool AddTask(Callback* task);

    // Add delayed task, the task will be executed 'delay' milliseconds
    // later, the task id can be used to remove it if this task has not
    // been executed.
    TaskId AddDelayedTask(int delay, Callback* task);

    // Add delayed task, the task will be executed in every 'interval'
    // milliseconds from 'delay' milliseconds later on, the task id can
    // be used to remove it if this task has not been executed.
    TaskId AddRepeatingTask(int delay, int interval, Callback* task);

    // Remove a task in synchronized way.
    // DO NOT call this method with a thread holding a lock needed by
    // the callback of this task! the threading model in the task runner
    // is asynchronized, when calling RemoveTask to remove a task, it
    // may is executing. In this case, there isn't a safe way to remove
    // it, the calling thread will be blocked until the task finishes.
    // If it returns true, it means the task has been removed success.
    // If it returns false, it means the task has finished when trying
    // to remove, or the task runner has beed shutdown.
    bool RemoveTask(TaskId task_id);

private:
    bool IsRunning() const;
    int64_t AddTaskEntryInternal(int delay,
                                 int internal,
                                 bool repeating,
                                 Callback* task);
    // Callback for delayed or repeating task from users
    void OnTimeout(detail::TaskEntry* entry, int64_t id);

    // Callback for handling pending timeouted user tasks, which should be
    // appended into threadpool, but the threadpool was full, we need to
    // store them and retry later.
    void OnHandlePendingTimeoutedTasks(int64_t id);

    void RunTask(detail::TaskEntry* entry);
    void RunTaskOnce(detail::TaskEntry* entry);
    void RunExtraTaskOnce(detail::TaskEntry* entry);

    detail::TaskEntry* GetTaskEntry(int64_t id);
    detail::TaskEntry* GetExtraTaskEntry(int64_t id);
    inline detail::TaskEntry* AcquireTaskEntry();
    void ReleaseTaskEntry(detail::TaskEntry* entry);
    void ReleaseTaskEntryExtra(detail::TaskEntry* entry);

    bool WaitForSingleTask(detail::TaskEntry* entry, int64_t id);

    inline static int32_t GetIndex(int64_t id);

private:
    enum Status
    {
        kStopped = 0,
        kRunning = 1,
        kStopping = 2
    };

private:
    typedef RestrictMutex LockType;
    typedef IntrusiveDList<detail::TaskEntry,
                           &detail::TaskEntry::mLinkNode,
                           false> ListType;

    static const int kEntryLimit;

    Status mStatus;

    int32_t mThreadNumLimit;
    ThreadPool *mThreadPool;
    TimeoutQueue *mTimeoutQueue;

    int32_t mEntryLimit;
    detail::TaskEntry** mTaskEntries;

    // Extra entries is for a very infrequent case, they are always in use.
    LockType mLockExtra;
    ListType mExtraEntries;
    FreeList<detail::TaskEntry, &detail::TaskEntry::mNextFreeNode>
            mTaskEntryCache;

    std::list<Callback*> mPendingTimeoutedTasks;

    __attribute__((aligned(64))) SpinRWLock mLock;
};

} // namespace stone
} // namespace apsara

#endif // STONE_THREADING_RUNNER_TASK_RUNNER_H
