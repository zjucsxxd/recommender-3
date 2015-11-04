// Copyright (c) 2013, Alibaba Inc.
// All rights reserved.
//
// Author: DongPing HUANG <dongping.huang@alibaba-inc.com>
// Created: 2013/05/15
// Description:

#ifndef STONE_UTILITY_MEMBER_TO_OBJECT_H
#define STONE_UTILITY_MEMBER_TO_OBJECT_H

namespace apsara {
namespace stone {

// GLOBAL_NOLINT(readability/casting)
template<typename T, typename M>
T* MemberToObject(M* member, M T::*mem_ptr)
{
    ptrdiff_t offset =
        (ptrdiff_t)&reinterpret_cast<const volatile char&>((((T*)0)->*mem_ptr));
    T* obj = (T*)((size_t)member - (size_t)offset);
    return obj;
}

} // namespace stone
} // namespace apsara

#endif // STONE_UTILITY_MEMBER_TO_OBJECT_H
