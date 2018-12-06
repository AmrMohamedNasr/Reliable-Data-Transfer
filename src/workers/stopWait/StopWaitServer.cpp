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
	uint32_t seq_rand = 0;
	struct sockaddr_in clAddr;
	struct timeval tv;
	struct timeval sendTime;
	while(dataFeeder->hasNext()) {
		struct packet_core_data packet_data = dataFeeder->getNextDataSegment();
		struct packet packet = create_data_packet( &packet_data, seq_rand);
		tv.tv_sec = TIMEOUT;
		tv.tv_usec = 0;
		gettimeofday(&sendTime, NULL);
		if (send_packet(sendSocket, clientAddr, &packet)) {
			tv.tv_sec = TIMEOUT;
			tv.tv_usec = 0;
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
				cout << "Time out occurred" << endl;
				tv.tv_sec = TIMEOUT;
				tv.tv_usec = 0;
				struct timeval sendTime;
				gettimeofday(&sendTime, NULL);
				if (!resend_packet(sendSocket, clientAddr, packet, &seq_rand, tv, sendTime)) {
					cout << "Ending program.." << endl;
					return;
				}
			} else if (error) {
				perror("Reception Error ");
				cout << "Error occurred on receiving packet..." << endl;
				cout << "Ending program.." << endl;
				return;
			} else if (verifyChecksumAck(&ack_packet) && ack_packet.ackno == seq_rand + 1){
				seq_rand++;
			} else {
				goto bad_pack;
			}
		} else {
			cout << "Error on sending..." << endl;
			return;
		}
	}
	struct packet_core_data packet_data;
	memset(&packet_data, 0, sizeof(struct packet_core_data));
	struct packet packet = create_data_packet( &packet_data, seq_rand);
	if (send_packet(sendSocket, clientAddr, &packet)) {
		tv.tv_sec = TIMEOUT;
		tv.tv_usec = 0;
		struct timeval sendTime;
		gettimeofday(&sendTime, NULL);
		bad_pack_2:
		bool mini_timeout = update_remaining_timeout(&tv, &sendTime);
		struct ack_packet ack_packet;
		if (!mini_timeout) {
			 ack_packet = receive_ack_packet(
				sendSocket, (struct sockaddr *)&clAddr, &error, &time_out, tv);
		}
		if (time_out || mini_timeout) {
			cout << "Time out occurred" << endl;
			tv.tv_sec = TIMEOUT;
			tv.tv_usec = 0;
			struct timeval sendTime;
			gettimeofday(&sendTime, NULL);
			if (!resend_packet(sendSocket, clientAddr, packet, &seq_rand, tv, sendTime)) {
				cout << "Ending program.." << endl;
				return;
			}
		} else if (error) {
			perror("Reception Error ");
			cout << "Error occurred on receiving packet..." << endl;
			cout << "Ending program.." << endl;
			return;
		} else if (verifyChecksumAck(&ack_packet) && ack_packet.ackno == seq_rand + 1){
			seq_rand++;
		} else {
			goto bad_pack_2;
		}
	} else {
		cout << "Error on sending..." << endl;
		return;
	}
}

bool StopWaitServer::resend_packet(int sendSocket, const struct sockaddr * clientAddr,
		struct packet packet, uint32_t *seq_no, struct timeval tv, struct timeval sendTime) {
	int i = 1;
	struct sockaddr_in clAddr;
	while(i <= RETRIES) {
		cout << "Retrying again for "<< i << "out of " << RETRIES << " retries." << endl;
		if (send_packet(sendSocket, clientAddr, &packet)) {
			tv.tv_sec = TIMEOUT;
			tv.tv_usec = 0;
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
				cout << "Time out occurred..." << endl;
			} else if (error) {
				cout << "Error occurred on receiving packet..." << endl;
			} else if (!verifyChecksumAck(&ack_packet) || ack_packet.ackno != *seq_no + 1) {
				goto bad_pack;
			} else {
				*seq_no = *seq_no + 1;
				return true;
			}
		} else {
			cout << "Error on sending..." << endl;
			return false;
		}
		i++;
	}
	return false;
}


