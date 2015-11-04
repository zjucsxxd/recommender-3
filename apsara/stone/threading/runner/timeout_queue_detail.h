// Copyright (c) 2013, Alibaba Inc.
// All right reserved.
//
// Author: DongPing HUANG <dongping.huang@alibaba-inc.com>
// Created: 2013/07/30
// Description:

#ifndef STONE_THREADING_RUNNER_TIMEOUT_QUEUE_DETAIL_H
#define STONE_THREADING_RUNNER_TIMEOUT_QUEUE_DETAIL_H

#include <cassert>
#include <functional>
#include <queue>
#include <vector>
#include "stone/base/closure.h"
#include "stone/containers/intrusive_list.h"

namespace apsara {
namespace stone {
namespace detail {

struct TimerEvent
{
    typedef Closure<void, int64_t> Callback;
    // fields
    // pay attention to alignment to save space
    LinkNode mLinkNode;
    int64_t mExpireTime;
    Callback* mCallback;
    int32_t mInterval;      // in milliseconds
    uint16_t mVersion;
    bool mValid;
    bool mRepeating;

    void Reset()
    {
        mExpireTime = -1;
        mCallback = 0;
        mInterval = -1;
        ++mVersion;
        mValid = false;
        mRepeating = false;
    }
    bool operator<(const TimerEvent& that) const
    {
        return this->mExpireTime > that.mExpireTime;
    }
    TimerEvent()
    {
        mVersion = -1;
        Reset();
    }
    ~TimerEvent()
    {
        delete mCallback;
        Reset();
        mVersion = -1;
    }
};

struct TimerEventCompare :
    public std::binary_function<TimerEvent*, TimerEvent*, bool>
{
    // compare method
    bool operator()(TimerEvent* left, TimerEvent* right) const
    {
        return left->operator<(*right);
    }
};

template<typename T,
         typename Sequence = std::vector<T>,
         typename Compare  = std::less<typename Sequence::value_type> >
class PriorityQueue : public std::priority_queue<T, Sequence, Compare>
{
public:
    Sequence* GetSequence()
    {
        return &this->c;
    }
    void Swap(Sequence* s)
    {
        assert(s);
        assert(std::__is_heap(s->begin(), s->end(), Compare()));
        this->c.swap(*s);
    }
};

} // namespace detail
} // namespace stone
} // namespace apsara

#endif // STONE_THREADING_RUNNER_TIMEOUT_QUEUE_DETAIL_H
