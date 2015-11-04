// Copyright (c) 2013, Alibaba Inc.
// All rights reserved.
//
// Author: DongPing HUANG <dongping.huang@alibaba-inc.com>
// Created: 2013/05/15
// Description:

#ifndef STONE_UTILITY_STL_UTIL_H
#define STONE_UTILITY_STL_UTIL_H

namespace apsara {
namespace stone {

template<class ForwardIterator>
void STLDeleteContainerPointers(ForwardIterator begin, ForwardIterator end)
{
    while (begin != end) {
        ForwardIterator temp = begin;
        ++begin;
        delete *temp;
    }
}

template<class T>
void STLDeleteElements(T* container)
{
    if (!container)
        return;
    STLDeleteContainerPointers(container->begin(), container->end());
    container->clear();
}

template <class ForwardIterator>
void STLDeleteContainerSecondPointers(ForwardIterator begin,
                                      ForwardIterator end) {
    while (begin != end) {
        ForwardIterator temp = begin;
        ++begin;
        delete temp->second;
    }
}

} // namespace stone
} // namespace apsara

#endif // STONE_UTILITY_STL_UTIL_H
