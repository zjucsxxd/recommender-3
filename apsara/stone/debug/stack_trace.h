// Copyright (c) 2013, Alibaba Inc.
// All right reserved.
//
// Author: DongPing HUANG <dongping.huang@alibaba-inc.com>
// Created: 2013/05/15
// Description:

#ifndef STONE_DEBUG_STACK_TRACE_H
#define STONE_DEBUG_STACK_TRACE_H

#include <iosfwd>
#include <string>

namespace apsara {
namespace stone {

class StackTrace
{
public:
    // ctor and dtor
    StackTrace();
    ~StackTrace();

    int ToString(std::string *stack) const;
    int Output(std::ostream* os) const;

private:
    static const int kMaxCallStack = 32;
    void* mTrace[kMaxCallStack];
    int mCount;
};

} // namespace stone
} // namespace apsara

#endif // STONE_DEBUG_STACK_TRACE_H
