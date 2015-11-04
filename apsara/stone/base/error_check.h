// Copyright (c) 2013, Alibaba Inc.
// All right reserved.
//
// Author: DongPing HUANG <dongping.huang@alibaba-inc.com>
// Created: 2013/05/15
// Description:

#ifndef STONE_BASE_ERROR_CHECK_H
#define STONE_BASE_ERROR_CHECK_H

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

namespace apsara {
namespace stone {

inline
static void CheckPthreadError(int status)
{
    if (status != 0) {
        fprintf(stdout, "PthreadError: %s\n", ::strerror(status));
        ::abort();
    }
}

inline
static int CheckPthreadTryLockError(int status, char* msg = NULL)
{
    if (status != 0) {
        if (status == EBUSY) {
            return false;
        } else {
            fprintf(stdout, "PthreadError: %s\n", ::strerror(status));
            ::abort();
        }
    }
    return true;
}

} // namespace stone
} // namespace apsara

#endif // STONE_BASE_ERROR_CHECK_H
