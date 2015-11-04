// Copyright (c) 2013, Alibaba Inc.
// All right reserved.
//
// Author: DongPing HUANG <dongping.huang@alibaba-inc.com>
// Created: 2013/12/23
// Description:

#ifndef STONE_MEMORY_REF_COUNTED_H
#define STONE_MEMORY_REF_COUNTED_H

#include <cstddef>
#include "stone/memory/uncopyable.h"
#include "stone/threading/atomic/atomic.h"

namespace apsara {
namespace stone {

template<class T>
class RefCounted : private stone::Uncopyable
{
public:
    RefCounted() : mRefCnt(0) {}

    void AddRef() { stone::AtomicIncrement(&mRefCnt); }

    size_t Release()
    {
        size_t refCnt = stone::AtomicDecrement(&mRefCnt);
        if (refCnt == 0) {
            delete static_cast<const T*>(this);
        }
        return refCnt;
    }

    bool HasOneRef() const
    {
        RefCounted* ptr = const_cast<RefCounted*>(this);
        return stone::AtomicGet(&ptr->mRefCnt) == 1U;
    }

protected:
    ~RefCounted() {}

private:
    size_t mRefCnt;
};

template<class T>
class RefCountedUnsafe : private stone::Uncopyable
{
public:
    RefCountedUnsafe() : mRefCnt(0) {}

    void AddRef() { ++mRefCnt; }

    size_t Release()
    {
        if (--mRefCnt == 0) {
            delete static_cast<const T*>(this);
        }
        return mRefCnt;
    }

    bool HasOneRef() const { return mRefCnt == 1U; }

protected:
    ~RefCountedUnsafe() {}

private:
    size_t mRefCnt;
};

} // namespace stone
} // namespace apsara

#endif // STONE_MEMORY_REF_COUNTED_H
