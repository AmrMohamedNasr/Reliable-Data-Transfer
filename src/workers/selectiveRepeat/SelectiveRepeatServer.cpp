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


SelectiveRepeatServer::~SelectiveRepeatServer() {

}


void SelectiveRepeatServer::send_message(DataFeeder *dataFeeder, float loss_prob,
				int sendSocket, const struct sockaddr * clientAddr, unsigned int window) {
	uint32_t seq_no = 0;
	base_seq_no = 0;
	sendOrder = list<uint32_t>();
	while(dataFeeder->hasNext()) {
		if (base_seq_no + window > seq_no) {
			struct packet_core_data packet_data = dataFeeder->getNextDataSegment();
			struct packet packet = create_data_packet( &packet_data, seq_no);
			bool sent;
			float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
			if (r < loss_prob) {
				sent = true;
				cout << "Packet " << seq_no << " dropped" << endl;
			} else {
				sent = send_packet(sendSocket, clientAddr, &packet);
			}
			if (sent) {
				struct timeval sendTime;
				gettimeofday(&sendTime, NULL);
				data_sent.insert(
						pair<uint32_t, struct packet>(seq_no, packet));
				seqnums_sent.insert(pair<uint32_t, struct timeval>(seq_no, sendTime));
				sendOrder.push_back(seq_no);
				sendOrderIterators[seq_no] = prev(sendOrder.end());
				seq_no++;
				while (hasData(sendSocket)) {
					receive_ack(sendSocket, window);
				}
				if (!updateTimers(sendSocket, clientAddr, loss_prob)) {
					return;
				}
			} else {
				cout << "Error on sending..." << endl;
				return;
			}
		} else {
			bool no_err = true;
			while (no_err && hasData(sendSocket)) {
				no_err = receive_ack(sendSocket, window);
			}
			if (!updateTimers(sendSocket, clientAddr, loss_prob)) {
				return;
			}
		}
	}
	while(!seqnums_sent.empty()) {
		bool no_err = true;
		while (no_err && hasData(sendSocket)) {
			no_err = receive_ack(sendSocket, window);
		}
		if (!updateTimers(sendSocket, clientAddr, loss_prob)) {
			return;
		}
	}
}

bool SelectiveRepeatServer::updateTimers(int sendSocket, const struct sockaddr * clientAddr, float loss_prob) {
	list<uint32_t>::iterator pack_id = sendOrder.begin();
	while (pack_id  != sendOrder.end()) {
		pair<uint32_t, struct timeval> not_acked = *seqnums_sent.find(*pack_id);
		struct timeval tv;
		tv.tv_sec = TIMEOUT;
		tv.tv_usec = 0;
		bool mini_timeout = update_remaining_timeout_nc(&tv, &not_acked.second);
		if (mini_timeout) {
			struct packet packet = data_sent.find(not_acked.first)->second;
			cout << "Timeout packet " << packet.seqno << endl;
			cout << "Retransmitting packet " << packet.seqno << endl;
			bool sent;
			float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
			if (r < loss_prob) {
				sent = true;
				cout << "Packet " << packet.seqno << " dropped" << endl;
			} else {
				sent = send_packet(sendSocket, clientAddr, &packet);
			}
			if (sent) {
				struct timeval sendTime;
				gettimeofday(&sendTime, NULL);
				seqnums_sent[not_acked.first] = sendTime;
				pack_id = sendOrder.erase(pack_id);
				sendOrder.push_back(packet.seqno);
				sendOrderIterators[packet.seqno] = prev(sendOrder.end());
			} else {
				cout << "Error on sending packets..." << endl;
				return false;
			}
		} else {
			return true;
		}
	}
	return true;
}

bool SelectiveRepeatServer::receive_ack(int sendSocket, unsigned int window) {
	struct timeval tv;
	tv.tv_sec = TIMEOUT;
	tv.tv_usec = 0;
	struct sockaddr_in clAddr;
	bool mini_timeout = update_remaining_timeout_nc(&tv, &(seqnums_sent[sendOrder.front()]));
	struct ack_packet ack_packet;
	if (!mini_timeout || hasData(sendSocket)) {
		mini_timeout = false;
		ack_packet = receive_ack_packet(sendSocket,
				(struct sockaddr *) &clAddr, &error, &time_out, tv);
	}
	if (time_out || mini_timeout) {
		// TODO update timer as in update timers function.
		cout << "Time out occurred " << sendOrder.front() << endl;
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

			//cout << "packet " << ack_packet.ackno << "is acknowledged" << endl;
			while (!acks.empty() && acks.find(base_seq_no) != acks.end()) {
				acks.erase(acks.find(base_seq_no));
				base_seq_no++;
				//cout << "forwarding window.\nBase Sequence Number = " << base_seq_no << endl;
			}
			seqnums_sent.erase(it);
			data_sent.erase(data_sent.find(it->first));
			sendOrder.erase(sendOrderIterators[it->first]);
			sendOrderIterators.erase(sendOrderIterators.find(it->first));
		}
		return true;
	}

	return false;
}
