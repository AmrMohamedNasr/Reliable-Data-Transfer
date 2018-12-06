/*
 * SelectiveRepeatServer.cpp
 *
 *  Created on: Dec 4, 2018
 *      Author: michael
 */
#include "SelectiveRepeatServer.h"
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
#include <random>
#define CLOSED_CYCLE 10
int randomWithProb(double p) {
    double rndDouble = (double)rand() / RAND_MAX;
    return rndDouble > p;
}

SelectiveRepeatServer::~SelectiveRepeatServer() {

}


void SelectiveRepeatServer::send_message(DataFeeder *dataFeeder, float loss_prob,
				int sendSocket, const struct sockaddr * clientAddr, unsigned int window) {
	uint32_t seq_no = 0;
	base_seq_no = 0;
	struct sockaddr_in clAddr;
	while(dataFeeder->hasNext()) {
		if (base_seq_no + window > seq_no) {
			struct packet_core_data packet_data = dataFeeder->getNextDataSegment();
			struct packet packet = create_data_packet( &packet_data, seq_no);
			struct timeval tv;
//			if (randomWithProb((double)(1 - loss_prob)) {
			if (send_packet(sendSocket, clientAddr, &packet)) {
				seq_no++;
				tv.tv_sec = TIMEOUT;
				tv.tv_usec = 0;
				data_sent.insert(
						pair<uint32_t, struct packet>(seq_no, packet));
				seqnums_sent.insert(pair<uint32_t, struct timeval>(seq_no, tv));
				while (hasData(sendSocket)) {
					receive_ack(sendSocket, window);
				}
				if (!updateTimers(sendSocket, clientAddr)) {
					return;
				}
			} else {
				cout << "Error on sending..." << endl;
				return;
			}
//			} else {
//				seq_no++;
//				data_sent.insert(pair<uint32_t, struct packet>(seq_no, packet));
//				seqnums_sent.insert(pair<uint32_t, struct timeval>(seq_no, tv));
//				while (hasData(sendSocket)) {
//					receive_ack(sendSocket, window);
//				}
//				if (!updateTimers(sendSocket, clientAddr)) {
//					return;
//				}
//			}
		} else {
			receive_ack(sendSocket, window);
			updateTimers(sendSocket, clientAddr);
		}
	}
}

bool SelectiveRepeatServer::updateTimers(int sendSocket, const struct sockaddr * clientAddr) {
	for (pair<uint32_t, struct timeval> not_acked: seqnums_sent) {
		struct timeval sendTime;
		gettimeofday(&sendTime, NULL);
		bool mini_timeout = update_remaining_timeout(&not_acked.second, &sendTime);
		if (mini_timeout) {
			struct packet packet = data_sent.find(not_acked.first)->second;
			if (send_packet(sendSocket, clientAddr, &packet)) {
				not_acked.second.tv_sec = TIMEOUT;
				not_acked.second.tv_usec = 0;
			} else {
				cout << "Error on sending packets..." << endl;
				return false;
			}
		}
	}
	return true;
}

bool SelectiveRepeatServer::receive_ack(int sendSocket, unsigned int window) {
	struct timeval tv;
	tv.tv_sec = TIMEOUT;
	tv.tv_usec = 0;
	struct sockaddr_in clAddr;
	struct ack_packet ack_packet = receive_ack_packet(sendSocket,
			(struct sockaddr *) &clAddr, &error, &time_out, tv);
	if (time_out) {
		cout << "Time out occurred" << endl;

		return false;
	} else if (error) {
		perror("Reception Error ");
		cout << "Error occurred on receiving packet..." << endl;
		cout << "Ending program.." << endl;

		return false;
	} else if (verifyChecksumAck(&ack_packet) && ack_packet.ackno >= base_seq_no
			&& ack_packet.ackno < base_seq_no + window) {
		if (seqnums_sent.find(ack_packet.ackno) != seqnums_sent.end()) {
			map<uint32_t, struct timeval>::iterator it = seqnums_sent.find(
					ack_packet.ackno);
			acks.insert(ack_packet.ackno);
			while (!acks.empty() && acks.find(base_seq_no) != acks.end()) {
				acks.erase(acks.find(base_seq_no));
				base_seq_no++;
			}
			seqnums_sent.erase(it);
			data_sent.erase(data_sent.find(it->first));
		}
		return true;
	}

	return false;
}
