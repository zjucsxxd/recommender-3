// Copyright (c) 2013, Alibaba Inc.
// All right reserved.
//
// Author: Ke TANG <ke.tang@alibaba-inc.com>
// Created: 2014/01/09
// Description:

#ifndef STONE_SYSTEM_PROCESS_H
#define STONE_SYSTEM_PROCESS_H

#include <string>

namespace apsara {
namespace stone {

class Process
{
public:
    static int GetProcessId();
    static std::string GetExecutablePath();

private:
    Process();
};

} // namespace stone
} // namespace apsara

#endif // STONE_SYSTEM_PROCESS_H
