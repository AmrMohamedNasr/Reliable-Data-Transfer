/*
 * StopWaitClient.cpp
 *
 *  Created on: Dec 4, 2018
 *      Author: michael
 */
#include "StopWaitClient.h"
#include <iostream>
#include <vector>
#include <sys/socket.h>
#include "../../utils/socketUtils.h"

StopWaitClient::~StopWaitClient() {

}


void StopWaitClient::recv_message(int socketFd, DataSink sink) {
	struct packet packet = receive_packet(socketFd, src_addr, &error, &time_out);

}


