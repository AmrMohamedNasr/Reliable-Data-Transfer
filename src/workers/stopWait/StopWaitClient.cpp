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
#include "../../web_models/ack_packet.h"
#include "../../data_managers/DataSink.h"


StopWaitClient::~StopWaitClient() {
}



void StopWaitClient::recv_message(int socketFd, DataSink *sink, unsigned int window) {
	memset(&src_addr, 0 , sizeof(sockaddr_in));
	uint32_t ackNo = 0;
	bool end = false;
	while (!end) {
		struct timeval tv;
		tv.tv_sec = 0;
		tv.tv_usec = 0;
		struct packet packet = receive_packet(socketFd, (struct sockaddr *)&src_addr, &error, &time_out, tv);
		if (error || time_out) {
			cout << "error occurred receiving packet" << endl;
		} else if (verifyChecksum(&packet) && packet.seqno == ackNo){
			if (packet.len != PCK_HEADER_SIZE) {
				struct packet_core_data core_data = extract_pure_data(&packet);
				sink->feed_next_data(&core_data, packet.seqno);
				ackNo = ackNo + 1;
				struct ack_packet ack_packet = create_ack_packet(
					ackNo);
				send_ack_packet(socketFd, (const struct sockaddr *)&src_addr, &ack_packet);
			} else {
				ackNo = ackNo + 1;
				struct ack_packet ack_packet = create_ack_packet(
					ackNo);
				send_ack_packet(socketFd, (const struct sockaddr *)&src_addr, &ack_packet);
				end = true;
			}
		}
	}
}


