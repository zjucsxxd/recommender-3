// Copyright (c) 2013, Alibaba Inc.
// All right reserved.
//
// Author: DongPing HUANG <dongping.huang@alibaba-inc.com>
// Created: 2013/05/15
// Description:

#ifndef STONE_STRING_STRING_NUMBER_CONVERSION_H
#define STONE_STRING_STRING_NUMBER_CONVERSION_H

#include <inttypes.h>
#include <string>
#include "stone/string/string_piece.h"

namespace apsara {
namespace stone {

bool StringToUint32(StringPiece input, uint32_t* output);
std::string ToString(int64_t n);

} // namespace stone
} // namespace apsara

#endif // STONE_STRING_STRING_NUMBER_CONVERSION_H
