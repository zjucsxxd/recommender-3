// Copyright (c) 2013, Alibaba Inc.
// All right reserved.
//
// Author: DongPing HUANG <dongping.huang@alibaba-inc.com>
// Created: 2013/07/29
// Description:

#ifndef STONE_THREADING_RUNNER_THREAD_POOL_H
#define STONE_THREADING_RUNNER_THREAD_POOL_H

#include <string>
#include "stone/base/closure.h"
#include "stone/base/uncopyable.h"
#include "stone/threading/runner/abstract_thread_pool.h"

namespace apsara {
namespace stone {

class ThreadPool : public AbstractThreadPool
{
public:
    enum Type
    {
        DynamicWorker = 0,
        FixedWorker = 1,
    };

public:
    ThreadPool(int threadNum,
               Type type = DynamicWorker,
               const std::string& name = "threadpool-default");

    ~ThreadPool()
    {
        delete mImpl;
    }

    bool Startup()
    {
        return mImpl->Startup();
    }
    bool Shutdown()
    {
        return mImpl->Shutdown();
    }

    // Add task. The tasks add into threadpool is not cancelable, they will
    // be executed definitely, unless the threadpool is shutdown before the
    // tasks be executed.
    int AddTask(Callback* closure)
    {
        return mImpl->AddTask(closure);
    }
    // Wait until all the tasks added into the threadpool are executed. But
    // the client cannot assume the threadpool definitely are idle, unless
    // it sures that there isn't any more tasks added when calling WaitFor-
    // Idle().
    void WaitForIdle()
    {
        mImpl->WaitForIdle();
    }

    // Get pending task numbers.
    int GetQueueLength()
    {
        return mImpl->GetQueueLength();
    }
    // Get live worker thread number in the threadpool.
    int GetThreadNum() const
    {
        return mImpl->GetThreadNum();
    }
    // Get the thread numbers of the worker threads which is not idle.
    int GetBusyThreadNum() const
    {
        return mImpl->GetBusyThreadNum();
    }

private:
    AbstractThreadPool* mImpl;
};

} // namespace stone
} // namespace apsara

#endif // STONE_THREADING_RUNNER_THREAD_POOL_H
