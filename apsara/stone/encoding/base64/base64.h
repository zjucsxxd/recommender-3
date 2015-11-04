// Copyright (c) 2014, Alibaba Inc.
// All right reserved.
//
// Author: DongPing HUANG <dongping.huang@alibaba-inc.com>
// Created: 2014/02/27
// Description: Port from common library of apsara.
//      Define the interface of base64 codec.

#ifndef STONE_ENCODING_BASE64_BASE64_H
#define STONE_ENCODING_BASE64_BASE64_H

#include <string>
#include "stone/string/string_piece.h"

namespace apsara {
namespace stone {

bool Base64Encode(const StringPiece& input, std::string* output);
bool Base64Encode(const StringPiece& input, char* output, size_t* len);

bool Base64Decode(const StringPiece& input, std::string* output);
bool Base64Decode(const StringPiece& input, char* output, size_t* len);

} // namespace stone 
} // namespace apsara

#endif // STONE_ENCODING_BASE64_BASE64_H

