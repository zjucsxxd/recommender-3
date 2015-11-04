// Copyright (c) 2013, Alibaba Inc.
// All right reserved.
//
// Author: DongPing HUANG <dongping.huang@alibaba-inc.com>
// Created: 2013/05/15
// Description:

#ifndef STONE_THREADING_LOCKFREE_FREE_LIST_H
#define STONE_THREADING_LOCKFREE_FREE_LIST_H

#include <stddef.h>
#include <stdint.h>
#include <algorithm>
#include <vector>
#include "stone/containers/intrusive_list.h"
#include "stone/threading/atomic/atomic.h"
#include "stone/threading/lockfree/lockfree_detail.h"
#include "stone/utility/member_to_object.h"

namespace apsara {
namespace stone {

template<typename T,
         LinkNode T::*M = &T::mLinkNode>
class FreeList
{
public:
    FreeList();
    ~FreeList();

    inline bool Push(T* obj);
    inline bool Pop(T** obj);
    size_t Size() const;
    bool Empty() const;

private:
    __attribute__((aligned(64))) uint64_t mHead;
    __attribute__((aligned(64))) size_t mSize;
};

template<typename T,
         LinkNode T::*M>
FreeList<T, M>::FreeList()
  : mHead(0), // NOLINT(whitespace/indent)
    mSize(0)
{
}

template<typename T,
         LinkNode T::*M>
FreeList<T, M>::~FreeList()
{
}

template<typename T,
         LinkNode T::*M>
bool FreeList<T, M>::Push(T* elem)
{
    uint64_t head;
    uint64_t newhead;
    do {
        head = mHead;
        newhead = detail::MakePointer(elem, detail::GetTag(head) + 1);
        T* node = reinterpret_cast<T*>(detail::GetPointer(head));
        (elem->*M).next = &(node->*M);
    } while (!AtomicCompareExchange(&mHead, newhead, head));
    AtomicIncrement(&mSize);
    return true;
}

template<typename T,
         LinkNode T::*M>
bool FreeList<T, M>::Pop(T** elem)
{
    uint64_t head;
    uint64_t newhead;
    do {
        head = mHead;
        if (__builtin_expect(detail::GetPointer(head) == NULL, 0)) {
            return false;
        }
        T* node = reinterpret_cast<T*>(detail::GetPointer(head));
        *elem = node;
        LinkNode* current = &(node->*M);
        LinkNode* next = current->next;
        newhead = detail::MakePointer(MemberToObject(next, M),
                                      detail::GetTag(head) + 1);
    } while (!AtomicCompareExchange(&mHead, newhead, head));
    AtomicDecrement(&mSize);
    return true;
}

template<typename T,
         LinkNode T::*M>
size_t FreeList<T, M>::Size() const
{
    return AtomicGet(&mSize);
}

template<typename T,
         LinkNode T::*M>
bool FreeList<T, M>::Empty() const
{
    return Size() == 0;
}

} // namespace stone
} // namespace apsara

#endif // STONE_THREADING_LOCKFREE_FREE_LIST_H
