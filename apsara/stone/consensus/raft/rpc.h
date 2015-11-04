// Copyright (c) 2013, Alibaba Inc.
// All right reserved.
//
// Author: DongPing HUANG <dongping.huang@alibaba-inc.com>
// Created: 2013/12/03
// Description:

#ifndef STONE_CONSENSUS_RAFT_RPC_H
#define STONE_CONSENSUS_RAFT_RPC_H

#include "protobuf/service.h"

// TODO(dongping.huang): typedef the following type as the ones in kuafu
//
class RpcServer;

typedef ::google::protobuf::RpcController RpcController;

typedef ::google::protobuf::RpcChannel RpcChannel;

typedef ::google::protobuf::Message Message;

#endif // STONE_CONSENSUS_RAFT_RPC_H
