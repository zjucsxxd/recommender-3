// Copyright (c) 2013, Alibaba Inc.
// All right reserved.
//
// Author: DongPing HUANG <dongping.huang@alibaba-inc.com>
// Created: 2013/05/15
// Description:

#ifndef STONE_THREADING_THIS_THREAD_H
#define STONE_THREADING_THIS_THREAD_H

#include <pthread.h>
#include <unistd.h>
#include <sys/syscall.h>

namespace apsara {
namespace stone {

class ThisThread {
public:
    static void SleepInMs(int ms)
    {
        ::usleep(ms * 1000);
    }

    static void SleepInUs(int us)
    {
        ::usleep(us);
    }

    static void Yield()
    {
        ::pthread_yield();
    }

    static int GetId()
    {
        static __thread pid_t tid = 0;
        if (tid == 0) {
            tid = ::syscall(SYS_gettid);
        }
        return tid;
    }

    static pthread_t GetThreadId()
    {
        return ::pthread_self();
    }

private:
    ThisThread();
};

} // namespace stone
} // namespace apsara

#endif // STONE_THREADING_THIS_THREAD_H
