// Copyright (c) 2013, Alibaba Inc.
// All right reserved.
//
// Author: DongPing HUANG <dongping.huang@alibaba-inc.com>
// Created: 2013/05/15
// Description:

#ifndef STONE_THREADING_LOCKFREE_LOCKFREE_DETAIL_H
#define STONE_THREADING_LOCKFREE_LOCKFREE_DETAIL_H

#include <stddef.h>
#include <stdint.h>

namespace apsara {
namespace stone {
namespace detail {

const uint32_t kLockFreeNullPointer = 0xFFFFFFFF;

typedef uint64_t Pointer;

inline uint64_t MakePointer(uint32_t index, uint32_t tag)
{
    uint64_t value = index;
    value <<= 32;
    value += tag;
    return value;
}
inline uint32_t Tag(uint64_t point)
{
    return point & 0xFFFFFFFF;
}

inline uint32_t Index(uint64_t point)
{
    return point >> 32;
}

union TaggedPointer
{
    uint64_t mValue;
    void* mPointer;
    struct
    {
        uint16_t mDummy[3];
        uint16_t mTag;
    };
};

const uint64_t kX64AddressSignBit = 0x0000800000000000;
const uint16_t kInvalidTag = 0xdead;

inline uint64_t MakePointer(void* address, uint16_t tag)
{
    TaggedPointer pointer;
    pointer.mValue =
        static_cast<uint64_t>(reinterpret_cast<intptr_t>(address));
    pointer.mTag = tag;
    return pointer.mValue;
}

inline void* GetPointer(uint64_t pointer)
{
    TaggedPointer tagged_pointer;
    tagged_pointer.mValue = pointer;
    tagged_pointer.mTag = (pointer & kX64AddressSignBit) ? 0xFFFF : 0;
    return  reinterpret_cast<void*>(
        static_cast<intptr_t>(tagged_pointer.mValue));
}

inline uint16_t GetTag(uint64_t pointer)
{
    TaggedPointer tagged_pointer;
    tagged_pointer.mValue = pointer;
    return tagged_pointer.mTag;
}

inline uint16_t GetNextTag(uint64_t pointer)
{
    TaggedPointer tagged_pointer;
    tagged_pointer.mValue = pointer;
    uint16_t tag = tagged_pointer.mTag + 1;
    if (__builtin_expect(tag == kInvalidTag, 0)) {
        ++tag;
    }
    return tag;
}

inline uint16_t GetPrevTag(uint64_t pointer)
{
    TaggedPointer tagged_pointer;
    tagged_pointer.mValue = pointer;
    uint16_t tag = tagged_pointer.mTag - 1;
    if (__builtin_expect(tag == kInvalidTag, 0)) {
        --tag;
    }
    return tag;
}

} // namespace detail
} // namespace stone
} // namespace apsara

#endif // STONE_THREADING_LOCKFREE_LOCKFREE_DETAIL_H
