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


void StopWaitClient::recv_message(int socketFd, DataSink sink) {
	memset(&src_addr, 0 , sizeof(sockaddr_in));
	bool end = false;
	while (!end) {
		struct packet packet = receive_packet(socketFd, (struct sockaddr *)&src_addr, &error, &time_out);
		if (error || time_out) {
			cout << "error occurred receiving packet" << endl;
		} else {
			if (packet.len != 0) {
				struct packet_core_data core_data = extract_pure_data(&packet);
				sink.feed_next_data(&core_data);
				struct ack_packet ack_packet = create_ack_packet(packet.seqno + packet.len + 1);
				// sending acknowledgment.
				sendto(socketFd, &ack_packet, sizeof(struct ack_packet*), SOCK_DGRAM,
					(struct sockaddr *) &src_addr, sizeof(src_addr));
			} else {
				end = true;
			}
		}
	}
}


