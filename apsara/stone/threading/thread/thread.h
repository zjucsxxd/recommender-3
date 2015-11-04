// Copyright (c) 2013, Alibaba Inc.
// All right reserved.
//
// Author: DongPing HUANG <dongping.huang@alibaba-inc.com>
// Created: 2013/05/15
// Description:

#ifndef STONE_THREADING_THREAD_THREAD_H
#define STONE_THREADING_THREAD_THREAD_H

#include <pthread.h>
#include "stone/base/closure.h"
#include "stone/threading/sync/waitable_event.h"

namespace apsara {
namespace stone {

typedef pthread_t ThreadHandle;

ThreadHandle CreateThread(::google::protobuf::Closure* func,
                          stone::WaitableEvent* wait = NULL,
                          const char* name = NULL);

} // namespace stone
} // namespace apsara

#endif // STONE_THREADING_THREAD_THREAD_H
