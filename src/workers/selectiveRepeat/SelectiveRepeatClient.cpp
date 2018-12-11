/*
 * SelectiveRepeatClient.cpp
 *
 *  Created on: Dec 4, 2018
 *      Author: michael
 */
#include "SelectiveRepeatClient.h"
#include <iostream>
#include <unistd.h>
#include <string.h>
#include "../../utils/socketUtils.h"
#include "../../web_models/packet_utils.h"
#include "../../web_models/ack_packet.h"
#include "../../data_managers/DataSink.h"
SelectiveRepeatClient::~SelectiveRepeatClient() {

}

void SelectiveRepeatClient::recv_message(int socketFd, DataSink *sink, unsigned int window) {
	memset(&src_addr, 0 , sizeof(sockaddr_in));
	base_ack_no = 0;
	uint32_t last_ack_no = 0xFFFFFFFF;
	bool end = false;
	while(!end || base_ack_no != last_ack_no) {
		struct timeval tv;
		tv.tv_sec = 0;
		tv.tv_usec = 0;
		struct packet packet = receive_packet(socketFd, (struct sockaddr *)&src_addr, &error, &time_out, tv);
		if (error || time_out) {
			cout << "error occurred receiving packet" << endl;
		} else if (verifyChecksum(&packet)) {
			if (packet.seqno < base_ack_no && packet.seqno >= base_ack_no - window) {
				struct ack_packet ack_packet = create_ack_packet(packet.seqno);
				send_ack_packet(socketFd, (const struct sockaddr *)&src_addr, &ack_packet);
			} else if (! (packet.seqno < base_ack_no) && ! (packet.seqno >= base_ack_no + window)) {
				if (packet.len != PCK_HEADER_SIZE) {
					struct packet_core_data core_data = extract_pure_data(&packet);
					if (!data_received.count(packet.seqno)) {
						data_received.insert(
							pair<uint32_t, struct packet_core_data>(packet.seqno, core_data));
					}
					struct ack_packet ack_packet = create_ack_packet(packet.seqno);
					send_ack_packet(socketFd, (const struct sockaddr *)&src_addr, &ack_packet);
					handleWindow(sink);
				} else {
					// check all window is acknowledged
					// or handle it form server to send empty packet after receiving all acks.
					struct ack_packet ack_packet = create_ack_packet(packet.seqno);
					send_ack_packet(socketFd, (const struct sockaddr *)&src_addr, &ack_packet);
					end = true;
					last_ack_no = packet.seqno;
				}
				//cout << "packet " << packet.seqno << "Received" << endl;
			}
		}

	}
}

void SelectiveRepeatClient::handleWindow(DataSink *sink) {
	while (!data_received.empty() && data_received.find(base_ack_no) != data_received.end()) {
		map<uint32_t, struct packet_core_data>::iterator it = data_received.find(base_ack_no);
		struct packet_core_data data = it->second;
		sink->feed_next_data(&data, base_ack_no);
		base_ack_no++;
		data_received.erase(it);
	}
}

