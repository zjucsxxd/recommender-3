// Copyright (c) 2013, Alibaba Inc.
// All right reserved.
//
// Author: DongPing HUANG <dongping.huang@alibaba-inc.com>
// Created: 2013/06/07
// Description: Port from common library of apsara.
//      Define the interface of crc checksum.

#ifndef STONE_ENCODING_CHECKSUM_CRC32_H
#define STONE_ENCODING_CHECKSUM_CRC32_H

#include <inttypes.h>

namespace apsara {
namespace stone {

/**
 * Calculate checksum for an input byte stream,
 * result is stored in checksum pool
 *
 * @param input Input data
 * @param inputLen Length of input data, in byte
 * @param checksum A 32-bit pool for storing checksum
 */
void DoCrc32(const void* input,
             const uint64_t inputLen,
             void* checksum);

/**
 * Check the correctness of a crc-checked byte stream
 *
 * @param poolIn Input data
 * @param inputBytesNum Length of input data, in byte
 * @param checksum Received checksum, used to compare with the newly
 * calculated checksum
 * @return true if no error detected, false if error detected
 */
bool CheckCrc32(const void* input,
                const uint64_t inputLen,
                const void* checksum);

// For CRC32C
typedef uint32_t (*Crc32cImpl)(uint32_t, const void*, uint64_t);

extern Crc32cImpl GetCrc32cImpl();

/** Calculate checksum for an input byte stream,
 *
 * @param initCrc Initial checksum value
 * @param data Input data to calculate checksum
 * @param length Length of input data
 * @return Calculated checksum
 */
inline uint32_t DoCrc32c(uint32_t init, const void* data, uint64_t length)
{
    static Crc32cImpl func = GetCrc32cImpl();
    return func(init, data, length);
}

} // namespace stone
} // namespace apsara

#endif // STONE_ENCODING_CHECKSUM_CRC32_H
