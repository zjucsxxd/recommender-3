// Copyright (c) 2013, Alibaba Inc.
// All right reserved.
//
// Author: DongPing HUANG <dongping.huang@alibaba-inc.com>
// Created: 2013/07/30
// Description:

#ifndef STONE_THREADING_RUNNER_TIMEOUT_QUEUE_H
#define STONE_THREADING_RUNNER_TIMEOUT_QUEUE_H

#include <map>
#include <string>
#include <vector>
#include "stone/base/uncopyable.h"
#include "stone/threading/lockfree/free_list.h"
#include "stone/threading/runner/timeout_queue_detail.h"
#include "stone/threading/sync/lock.h"
#include "stone/threading/sync/waitable_event.h"

namespace apsara {
namespace stone {

class TimeoutQueue : private Uncopyable
{
public:
    typedef Closure<void, int64_t> Callback;
    static const int64_t kInvalidEventId = -1;

    TimeoutQueue(int32_t max_queue_size = 65536,
                 const std::string& name = "default");
    ~TimeoutQueue();

    // delay is in milliseconds
    // Return positive integer on success, zero on fail
    int64_t Add(int32_t delay, Callback* callback);

    // delay and interval are in milliseconds
    // Repeating event, callback should be a permanent closure
    // Return positive integer on success, the return value is the id of the
    // timeout task, which could be used to remove the task.
    // Return zero on fail
    int64_t AddRepeating(int32_t delay,
                         int32_t interval,
                         Callback* callback);

    // Remove an event synchronically. It's dangrous to remove event in this
    // way.
    // If the working thread for the event manager is calling the callback
    // function, the caller thread of Remove have no chance but wait for
    // the calling finish. But the caller thread may hold the locks that the
    // working thread needs to finish the callback, then DEAD LOCK happen!
    // Return true if the event is removed. return false if it is not found.
    bool Remove(int64_t id);

    // Remove an event asynchronically. If cannot find the event or the worker
    // thread is handling the event, return false. If it's removed, return
    // true.
    // That's to say, if this function return false, the caller must use some
    // others way to determine the event has finished.
    bool AsyncRemove(int64_t id);

    void WaitForIdle();

protected:
    // for test purpose
    size_t GetCachedItemCount();

private:
    typedef detail::TimerEvent Event;

    bool Startup();
    bool Shutdown();

    bool IsRunning() const {
        return mStarted && !mStopped;
    }

    bool AddEvent(Event* event);
    bool RemoveEvent(int64_t id);
    void WorkerRoutine();
    void ExecuteOneEvent(Event* event);

    bool GetNextExpiredEvent(int64_t timestamp, Event** event);
    int64_t GetNextExpireTime();

    // Get a Event instance from cached free-list, or allocate a new one
    bool AcquireEvent(Event** event);
    // Store the Event instance into cached free-list, or release the
    // memory if there are enough instances cached already.
    void ReleaseEvent(Event* event);
    // Check the total events and invalid percentage to determine trim or not
    bool NeedToTrim();
    // Trim off those events labelled as invalid
    void TrimInvalidEventsIfNeeded();

    int64_t GetEventId(const Event* event) const;
    static int64_t MakeEventId(int32_t index, uint16_t version);
    static int32_t GetIndex(int64_t event_id);
    static uint16_t GetVersion(int64_t event_id);

private:
    typedef detail::PriorityQueue<Event*,
                                  std::vector<Event*>,
                                  detail::TimerEventCompare> QueueType;
    typedef RestrictMutex LockType;

    std::string mName;

    LockType mLock;
    bool mStarted;
    bool mStopped;
    volatile bool mWorkerFinished;

    int32_t mQueueLimit;

    static const int64_t kMaxExpireTime;
    int64_t mRunningId; // the event which is running
    int64_t mNextExpireTime;

    WaitableEvent mEvent;
    QueueType mTaskQueue;
    FreeList<Event> mTasksPending;
    int64_t mEventCount;
    // for cache purpose
    FreeList<Event> mFreeEventList;
    Event* mEventBuffer;
};

} // namespace stone
} // namespace apsara

#endif // STONE_THREADING_RUNNER_TIMEOUT_QUEUE_H
