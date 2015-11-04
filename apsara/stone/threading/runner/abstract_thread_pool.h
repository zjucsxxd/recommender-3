// Copyright (c) 2013, Alibaba Inc.
// All right reserved.
//
// Author: DongPing HUANG <dongping.huang@alibaba-inc.com>
// Created: 2013/07/29
// Description:

#ifndef STONE_THREADING_RUNNER_ABSTRACT_THREAD_POOL_H
#define STONE_THREADING_RUNNER_ABSTRACT_THREAD_POOL_H

#include "stone/base/closure.h"
#include "stone/base/uncopyable.h"

namespace apsara {
namespace stone {

class AbstractThreadPool : private Uncopyable
{
public:
    typedef google::protobuf::Closure Callback;

    static const int kNotRunning = -1;
    static const int kQueueFull = -2;

    virtual ~AbstractThreadPool() {}

    /**
     * Startup enable the threadpool, and Shutdown turn it off. The
     * threadpool handles the tasks only when it's enabled.
     */
    virtual bool Startup() = 0;
    virtual bool Shutdown() = 0;

    /**
     * Add task. The tasks add into threadpool is not cancelable, they will
     * be executed definitely, unless the threadpool is shutdown before the
     * tasks be executed.
     */
    virtual int AddTask(Callback* task) = 0;
    /**
     * Wait until all the tasks added into the threadpool are executed. But
     * the client cannot assume the threadpool definitely are idle, unless
     * it sures that there isn't any more tasks added when calling WaitFor-
     * Idle().
     */
    virtual void WaitForIdle() = 0;

    // Get pending task numbers.
    virtual int GetQueueLength() = 0;
    // Get live worker thread number in the threadpool.
    virtual int GetThreadNum() const = 0;
    // Get the thread numbers of the worker threads which is not idle.
    virtual int GetBusyThreadNum() const = 0;
};

} // namespace stone
} // namespace apsara

#endif // STONE_THREADING_RUNNER_ABSTRACT_THREAD_POOL_H
