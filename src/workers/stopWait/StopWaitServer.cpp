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

#define NO_OF_TRIES 3
StopWaitServer::~StopWaitServer() {

}


void StopWaitServer::send_message(DataFeeder dataFeeder, float loss_prob,
				int sendSocket, const struct sockaddr * clientAddr, unsigned int window) {
	uint32_t seq_rand = (rand() % 100) + 1;
	while(dataFeeder.hasNext()) {
		struct packet_core_data packet_data = dataFeeder.getNextDataSegment();
		struct packet packet = create_data_packet( &packet_data, seq_rand);
		if (send_packet(sendSocket, clientAddr, &packet)) {
			struct ack_packet ack_packet = receive_ack_packet(
					sendSocket, clientAddr, &error, &time_out);
			if (time_out) {
				cout << "Time out occurred" << endl;
				if (!resend_packet(sendSocket, clientAddr, packet, &seq_rand)) {
					cout << "Ending program.." << endl;
					return;
				}
			} else if (error) {
				cout << "Error occurred on receiving packet..." << endl;
				if (!resend_packet(sendSocket, clientAddr, packet, &seq_rand)) {
					cout << "Ending program.." << endl;
					return;
				}
			}
		} else {
			cout << "Error on sending..." << endl;
			return;
		}
	}

}

bool StopWaitServer::resend_packet(int sendSocket, const struct sockaddr * clientAddr,
		struct packet packet, uint32_t *seq_no) {
	int i = 1;
	while(i <= NO_OF_TRIES) {
		cout << "Retrying again for "<< i << "out of " << NO_OF_TRIES << " retries." << endl;
		if (send_packet(sendSocket, clientAddr, &packet)) {
			struct ack_packet ack_packet = receive_ack_packet(
				sendSocket, clientAddr, &error, &time_out);
			if (time_out) {
				cout << "Time out occurred..." << endl;
			} else if (error) {
				cout << "Error occurred on receiving packet..." << endl;
			} else {
				*seq_no = ack_packet.ackno;
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


