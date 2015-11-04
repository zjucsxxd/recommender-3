// Copyright (c) 2013, Alibaba Inc.
// All right reserved.
//
// Author: DongPing HUANG <dongping.huang@alibaba-inc.com>
// Created: 2013/11/26
// Description:

#ifndef STONE_STRING_STRING_SPLIT_H
#define STONE_STRING_STRING_SPLIT_H

#include <string>
#include <vector>

namespace apsara {
namespace stone {

void SplitString(const std::string& str,
                 const std::string& s,
                 std::vector<std::string>* r);

} // namespace stone
} // namespace apsara

#endif // STONE_STRING_STRING_SPLIT_H
