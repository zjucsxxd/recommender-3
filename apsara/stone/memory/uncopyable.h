// Copyright (c) 2013, Alibaba Inc.
// All right reserved.
//
// Author: DongPing HUANG <dongping.huang@alibaba-inc.com>
// Created: 2013/05/15
// Description:

#ifndef STONE_MEMORY_UNCOPYABLE_H
#define STONE_MEMORY_UNCOPYABLE_H

namespace apsara {
namespace stone {

class Uncopyable {
protected:
    Uncopyable() {}
    ~Uncopyable() {}

private:
    Uncopyable(const Uncopyable& o);
    Uncopyable& operator=(const Uncopyable& o);
};

} // namespace stone
} // namespace apsara

#endif // STONE_MEMORY_UNCOPYABLE_H
