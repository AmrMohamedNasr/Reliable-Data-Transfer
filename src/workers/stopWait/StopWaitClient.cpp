/*
 * StopWaitClient.cpp
 *
 *  Created on: Dec 4, 2018
 *      Author: michael
 */
#include "StopWaitClient.h"
#include <iostream>
#include <unistd.h>
#include <string.h>
#include "../../utils/socketUtils.h"
#include "../../web_models/packet_utils.h"

StopWaitClient::~StopWaitClient() {

}


void StopWaitClient::recv_message(int socketFd, DataSink sink) {
	memset(&src_addr, 0 , sizeof(sockaddr_in));
	memset(&dest_addr, 0 , sizeof(sockaddr_in));
	struct packet packet = receive_packet(socketFd, (struct sockaddr *)&src_addr, &error, &time_out);
	if (error || time_out) {

	} else {
		sink.feed_next_data(&extract_pure_data(packet));

	}
}


