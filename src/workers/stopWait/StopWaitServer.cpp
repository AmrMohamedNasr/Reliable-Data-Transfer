/*
 * StopWaitServer.cpp
 *
 *  Created on: Dec 4, 2018
 *      Author: amrnasr
 */
#include "StopWaitServer.h"
#include <iostream>
#include <unistd.h>
#include <string.h>
#include <cstdlib>
#include "../../utils/socketUtils.h"
#include "../../web_models/packet_utils.h"
#include "../../web_models/ack_packet.h"
#include "../../data_managers/DataFeeder.h"
#include <netinet/in.h>
#include <sys/time.h>

StopWaitServer::~StopWaitServer() {

}


void StopWaitServer::send_message(DataFeeder *dataFeeder, float loss_prob,
				int sendSocket, const struct sockaddr * clientAddr, unsigned int window) {
	uint32_t seq_no = 0;
	struct sockaddr_in clAddr;
	struct timeval tv;
	struct timeval sendTime;
	while(dataFeeder->hasNext()) {
		struct packet_core_data packet_data = dataFeeder->getNextDataSegment();
		struct packet packet = create_data_packet( &packet_data, seq_no);
		tv.tv_sec = TIMEOUT_SEC;
		tv.tv_usec = TIMEOUT_MSEC;
		gettimeofday(&sendTime, NULL);
		bool sent;
		float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		if (r < loss_prob) {
			sent = true;
			cout << "Packet " << seq_no << " dropped" << endl;
		} else {
			sent = send_packet(sendSocket, clientAddr, &packet);
		}
		if (sent) {
			tv.tv_sec = TIMEOUT_SEC;
			tv.tv_usec = TIMEOUT_MSEC;
			struct timeval sendTime;
			gettimeofday(&sendTime, NULL);
			bad_pack:
			bool mini_timeout = update_remaining_timeout(&tv, &sendTime);
			struct ack_packet ack_packet;
			if (!mini_timeout) {
				 ack_packet = receive_ack_packet(
					sendSocket, (struct sockaddr *)&clAddr, &error, &time_out, tv);
			}
			if (time_out || mini_timeout) {
				cout << "Time out occurred " << packet.seqno << endl;
				tv.tv_sec = TIMEOUT_SEC;
				tv.tv_usec = TIMEOUT_MSEC;
				struct timeval sendTime;
				gettimeofday(&sendTime, NULL);
				if (!resend_packet(sendSocket, clientAddr, packet, &seq_no, tv, sendTime, loss_prob)) {
					cout << "Ending program.." << endl;
					return;
				}
			} else if (error) {
				perror("Reception Error ");
				cout << "Error occurred on receiving packet..." << endl;
				cout << "Ending program.." << endl;
				return;
			} else if (verifyChecksumAck(&ack_packet) && ack_packet.ackno == seq_no){
				seq_no++;
			} else {
				goto bad_pack;
			}
		} else {
			cout << "Error on sending..." << endl;
			return;
		}
	}
}

bool StopWaitServer::resend_packet(int sendSocket, const struct sockaddr * clientAddr,
		struct packet packet, uint32_t *seq_no, struct timeval tv, struct timeval sendTime, float loss_prob) {
	struct sockaddr_in clAddr;
	bool retrans_success = false;
	while(!retrans_success) {
		cout << "Retransmitting packet "<< packet.seqno << endl;
		bool sent;
		float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		if (r < loss_prob) {
			cout << "Packet " << *seq_no << " Dropped" << endl;
			sent = true;
		} else {
			sent = send_packet(sendSocket, clientAddr, &packet);
		}
		if (sent) {
			tv.tv_sec = TIMEOUT_SEC;
			tv.tv_usec = TIMEOUT_MSEC;
			struct timeval sendTime;
			gettimeofday(&sendTime, NULL);
			bad_pack:
			bool mini_timeout = update_remaining_timeout(&tv, &sendTime);
			struct ack_packet ack_packet;
			if (!mini_timeout) {
				ack_packet = receive_ack_packet(
						sendSocket, (struct sockaddr *)&clAddr, &error, &time_out, tv);
			}
			if (time_out || mini_timeout) {
				cout << "Time out occurred " << packet.seqno << endl;
			} else if (error) {
				cout << "Error occurred on receiving packet..." << endl;
			} else if (!verifyChecksumAck(&ack_packet) || ack_packet.ackno != *seq_no) {
				goto bad_pack;
			} else {
				*seq_no = *seq_no + 1;
				retrans_success = true;
			}
		} else {
			cout << "Error on sending..." << endl;
			return false;
		}
	}
	return retrans_success;
}


