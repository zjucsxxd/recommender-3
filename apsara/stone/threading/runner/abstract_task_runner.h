// Copyright (c) 2013, Alibaba Inc.
// All right reserved.
//
// Author: DongPing HUANG <dongping.huang@alibaba-inc.com>
// Created: 2013/08/06
// Description:

#ifndef STONE_THREADING_RUNNER_ABSTRACT_TASK_RUNNER_H
#define STONE_THREADING_RUNNER_ABSTRACT_TASK_RUNNER_H

#include "stone/base/closure.h"
#include "stone/base/uncopyable.h"

namespace apsara {
namespace stone {

class ThreadPool;
class TimeoutQueue;

// TaskRunner is a pump for asynchronized tasks. It supports immediate
// task, delayed task, and repeating task.
// Because the task is executed in asynchronized way, DO NOT delete the
// objects refered by the callback before it finishes. You may need a way
// to record the tasks added into the task runner, and update the records
// in the callback.
class AbstractTaskRunner : private Uncopyable
{
public:
    typedef google::protobuf::Closure Callback;
    typedef int64_t TaskId;
    static const TaskId kInvalidTaskId = -1;

    virtual ~AbstractTaskRunner() {}

    virtual void Startup() = 0;
    virtual void Shutdown() = 0;

    // Add immediate task, which is not cancelable. The immediate tasks
    // will be executed unless the task run is shutdown before the task
    // executed.
    virtual bool AddTask(Callback* task) = 0;

    // Add delayed task, the task will be executed 'delay' milliseconds
    // later, the task id can be used to remove it if this task has not
    // been executed.
    virtual TaskId AddDelayedTask(int delay, Callback* task) = 0;

    // Add delayed task, the task will be executed in every 'interval'
    // milliseconds from 'delay' milliseconds later on, the task id can
    // be used to remove it if this task has not been executed.
    virtual TaskId AddRepeatingTask(int delay, int interval, Callback* task) = 0;

    // Remove a task in synchronized way.
    // DO NOT call this method with a thread holding a lock needed by
    // the callback of this task! the threading model in the task runner
    // is asynchronized, when calling RemoveTask to remove a task, it
    // may is executing. In this case, there isn't a safe way to remove
    // it, the calling thread will be blocked until the task finishes.
    // If it returns true, it means the task has been removed success.
    // If it returns false, it means the task has finished when trying
    // to remove, or the task runner has beed shutdown.
    virtual bool RemoveTask(TaskId task_id) = 0;
};

} // namespace stone
} // namespace apsara

#endif // STONE_THREADING_RUNNER_ABSTRACT_TASK_RUNNER_H
