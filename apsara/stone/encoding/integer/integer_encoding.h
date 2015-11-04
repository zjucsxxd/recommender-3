// Copyright (c) 2013, Alibaba Inc.
// All right reserved.
//
// Author: ChengYu DONG <chengyu.dong@alibaba-inc.com>
// Created: 2013/06/08
// Description:

#ifndef STONE_ENCODING_INTEGER_INTEGER_ENCODING_H
#define STONE_ENCODING_INTEGER_INTEGER_ENCODING_H

// Fixed-integers and variable-integers are encoded as bytes-sequence.
// Several static template functions are provided by interger T.
// T SHOULD be an integer type.

// TODO(chengyu.dong): To support StringPiece as input/output ASAP.

#include <stdint.h>
#include <string>

namespace apsara {
namespace stone {

// Fixed length encoding and decoding in fixed integer format.
// ONLY support little-endian byte order.
class FixedInt
{
public:
    template <typename T>
    static inline char* Encode(char* s, T x)
    {
        *reinterpret_cast<T*>(s) = x;
        return s;
    }

    // Do NOT CHECK validity, equality between length and sizeof(*x) here
    // for higher performance. It's user's responsibility to do that.
    template <typename T>
    static inline T Decode(const char* s)
    {
        return *reinterpret_cast<const T*>(s);
    }
};

// VariantInt is a class of serializing integers using one or more bytes.

// Variable length encoding and decoding in variant int format.
class VariantInt
{
public:
    // Encode interger 'x' to output buffer, the variable length as returned
    // value. Returns the encoded length.
    template <typename T>
    static size_t Encode(char* s, T x)
    {
        // Handle characters as unsigneds.
        unsigned char* p = reinterpret_cast<unsigned char*>(s);
        while (x >= 0x80) {
            *p++ = (x & 0x7f) | 0x80;
            x >>= 7;
        }
        *p++ = x;
        return reinterpret_cast<char*>(p) - s;
    }

    // Decode input buffer to a variant int. Return false if the maximum int by
    // T exceeds.
    template <typename T>
    static bool Decode(const char* s, T* x, size_t limit = (size_t)-1,
                       size_t* offset = NULL)
    {
        // Max interger representing by T. sizeof(x) can't be larger than that
        // of uint64_t.  kIntMax is different according to signed and unsigned
        // integers.
        const unsigned char* p = reinterpret_cast<const unsigned char*>(s);
        uint64_t value = 0;
        size_t shift = 0;
        const size_t max = GetMaxCharsOfEncodedSize<T>();
        limit = (limit > max) ? max : limit;
        for (size_t i = 0; i < limit; ++i) {
            uint64_t c = *p++;
            value |= ((c & 0x7f) << shift);
            if ((c & 0x80) == 0) {
                // Maybe there still have remaining bytes, ignore them.
                *x = value;
                if (offset) *offset = i + 1;
                return true;
            }
            shift += 7;
        }
        // Decoding failed.
        return false;
    }

    // Encoding length.
    template <typename T>
    static size_t GetLength(T x)
    {
        size_t length = 1;
        while (x >= 0x80) {
            x >>= 7;
            ++length;
        }
        return length;
    }

    template <typename T>
    static inline size_t GetMaxCharsOfEncodedSize()
    {
        return (sizeof(T) * 8 + 6) / 7;
    }
};

} // namespace stone
} // namespace apsara

#endif // STONE_ENCODING_INTEGER_INTEGER_ENCODING_H
