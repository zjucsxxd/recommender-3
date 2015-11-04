// Copyright (c) 2014, Alibaba Inc.
// All right reserved.
//
// Author: Yongming WU <yongming.wym@alibaba-inc.com>
// Created: 2014/06/26
// Description: This file collects the utilities used
// by the local file system.

#ifndef STONE_IO_FILE_UTIL_H
#define STONE_IO_FILE_UTIL_H

#include <errno.h>
#include <string.h>
#include "stone/base/status.h"

namespace apsara {
namespace stone {

inline Status ConvertErrnoToStatus(int errnum)
{
    Status s;

    switch (errnum) {
    case EACCES:
        s = Status(Status::kPermissionDenied, ::strerror(errnum));
        break;
    case ENOENT:
    case ENOTDIR:
        s = Status(Status::kNotFound , ::strerror(errnum));
        break;
    case EISDIR:
    case ENOTEMPTY:
    case EEXIST:
        s = Status(Status::kFolderExist , ::strerror(errnum));
        break;
    case ENAMETOOLONG:
    case EINVAL:
        s = Status(Status::kInvalidArgument , ::strerror(errnum));
        break;
    case EMFILE:
    case ENFILE:
    case ENOMEM:
    case ENOSPC:
        s = Status(Status::kOutOfResource, ::strerror(errnum));
        break;
    case EBADF:
        s = Status(Status::kNotReady, ::strerror(errnum));
        break;
    case EAGAIN:
        s = Status(Status::kFileLocked, ::strerror(errnum));
        break;
    default:
        s = Status(Status::kOtherError, ::strerror(errnum));
        break;
    }

    return s;
}

} // namespace stone
} // namespace apsara

#endif // STONE_IO_FILE_UTIL_H
