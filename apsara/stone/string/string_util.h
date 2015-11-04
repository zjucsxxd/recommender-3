// Copyright (c) 2013, Alibaba Inc.
// All right reserved.
//
// Author: DongPing HUANG <dongping.huang@alibaba-inc.com>
// Created: 2013/11/26
// Description:

#ifndef STONE_STRING_STRING_UTIL_H
#define STONE_STRING_STRING_UTIL_H

#include <string>

namespace apsara {
namespace stone {

enum TrimOption
{
    kTrimNone = 0,
    kTrimLeading = 1 << 0,
    kTrimTrailing = 1 << 1,
    kTrimAll = kTrimLeading | kTrimTrailing,
};

void TrimWhitespace(const std::string& input,
                    TrimOption option,
                    std::string* output);

void ReplaceString(const std::string& input,
                   const std::string& old_val,
                   const std::string& new_val,
                   std::string* output);

inline std::string ReplaceString(const std::string& input,
                                 const std::string& old_val,
                                 const std::string& new_val)
{
    std::string output;
    ReplaceString(input, old_val, new_val, &output);
    return output;
}

std::string ToLowerCase(const std::string& input);
std::string ToUpperCase(const std::string& input);

} // namespace stone
} // namespace apsara

#endif // STONE_STRING_STRING_UTIL_H
