/*
 * selectiveRepeatCCServer.cpp
 *
 *  Created on: Dec 7, 2018
 *      Author: amrnasr
 */

#include "selectiveRepeatCCServer.h"
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

SelectiveRepeatCCServer::SelectiveRepeatCCServer() {
	dupAcks = vector<unsigned int>();
	time_out = false;
	error = false;
	base_seq_no = 0;
	sendOrder = list<uint32_t> ();
	sendOrderIterators = unordered_map<uint32_t, list<uint32_t>::iterator> ();
	seqnums_sent = map<uint32_t, struct timeval> ();
	data_sent = map<uint32_t, struct packet> ();
	acks = set<uint32_t> ();
	dupAckIndex = 0;
	successAcks = 0;
	seq_no = 0;
	out_of_window_packets = map<uint32_t, struct packet> ();
	transmission_rounds_window = vector<unsigned int> ();
	transmission_rounds_ssthres = vector<unsigned int> ();
	transmission_rounds = vector<float>();
	trans_round = 0;
	FileHandler handl = FileHandler();
	if (handl.check_file("control.cc")) {
		ifstream in("control.cc");
		unsigned int i;
		while (in >> i) {
			dupAcks.push_back(i);
		}
	}
}

SelectiveRepeatCCServer::~SelectiveRepeatCCServer() {

}


void write_congestion_rounds(ServerWorker *worker) {
	SelectiveRepeatCCServer serv = *((SelectiveRepeatCCServer *)worker);
	ofstream out("trans_rounds.cc");
	for (unsigned i = 0; i < serv.transmission_rounds_ssthres.size(); i++) {
		out << serv.transmission_rounds[i] << "\t" << serv.transmission_rounds_window[i] << "\t" << serv.transmission_rounds_ssthres[i] << endl;
	}
}

void SelectiveRepeatCCServer::send_message(DataFeeder *dataFeeder, float loss_prob,
				int sendSocket, const struct sockaddr * clientAddr, unsigned int org_window) {
	seq_no = 0;
	base_seq_no = 0;
	sendOrder = list<uint32_t>();
	trans_round = 1;
	unsigned int window = 1;
	unsigned int ssthress = org_window;
	unsigned int miniWin = 0;
	transmission_rounds_ssthres.push_back(ssthress);
	transmission_rounds_window.push_back(window);
	transmission_rounds.push_back(trans_round);
	trans_round++;
	while(dataFeeder->hasNext()) {
		if (base_seq_no + window > seq_no) {
			struct packet packet;
			if (out_of_window_packets.find(seq_no) == out_of_window_packets.end()) {
				struct packet_core_data packet_data = dataFeeder->getNextDataSegment();
				packet = create_data_packet( &packet_data, seq_no);
			} else {
				packet = out_of_window_packets[seq_no];
			}
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
					receive_ack(sendSocket, &window, &ssthress, &miniWin);
				}
				if (!updateTimers(sendSocket, clientAddr, loss_prob, &window,
						&ssthress, &miniWin)) {
					return;
				}
			} else {
				cout << "Error on sending..." << endl;
				return;
			}
		} else {
			bool no_err = true;
			while (no_err && hasData(sendSocket)) {
				no_err = receive_ack(sendSocket, &window,
						&ssthress, &miniWin);
			}
			if (!updateTimers(sendSocket, clientAddr, loss_prob, &window,
					&ssthress, &miniWin)) {
				return;
			}
		}
	}
	while(!seqnums_sent.empty()) {
		bool no_err = true;
		while (no_err && hasData(sendSocket)) {
			no_err = receive_ack(sendSocket, &window,
					&ssthress, &miniWin);
		}
		if (!updateTimers(sendSocket, clientAddr, loss_prob, &window,
				&ssthress, &miniWin)) {
			return;
		}
	}
}

bool SelectiveRepeatCCServer::updateTimers(int sendSocket, const struct sockaddr * clientAddr,
		float loss_prob, unsigned int * window, unsigned int *ssthres, unsigned int *miniWin) {
	list<uint32_t>::iterator pack_id = sendOrder.begin();
	while (pack_id  != sendOrder.end()) {
		pair<uint32_t, struct timeval> not_acked = *seqnums_sent.find(*pack_id);
		struct timeval tv;
		tv.tv_sec = TIMEOUT;
		tv.tv_usec = 0;
		bool mini_timeout = update_remaining_timeout_nc(&tv, &not_acked.second);
		if (mini_timeout) {
			window_decrease(window, ssthres, miniWin, true);
			if (data_sent.find(not_acked.first) != data_sent.end()) {
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
		} else {
			return true;
		}
	}
	return true;
}

bool SelectiveRepeatCCServer::receive_ack(int sendSocket, unsigned int* window,
		unsigned int *ssthres, unsigned int *miniWin) {
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
		cout << "Time out occurred" << sendOrder.front() << endl;
		window_decrease(window, ssthres, miniWin, true);
		return false;
	} else if (error) {
		perror("Reception Error ");
		cout << "Error occurred on receiving packet..." << endl;
		cout << "Ending program.." << endl;

		return false;
	} else if (verifyChecksumAck(&ack_packet) && ack_packet.ackno >= base_seq_no
			&& ack_packet.ackno < base_seq_no + *window) {
		if (seqnums_sent.find(ack_packet.ackno) != seqnums_sent.end()) {
			successAcks++;
			if (*window >= *ssthres) {
				*miniWin = *miniWin + 1;
				if (*miniWin == *window) {
					*window = *window + 1;
					*miniWin = 0;
					transmission_rounds_ssthres.push_back(*ssthres);
					transmission_rounds_window.push_back(*window);
					transmission_rounds.push_back(trans_round);
					trans_round++;
				}
			} else {
				*window += 1;
				if (*window == 2 * transmission_rounds_window.back()) {
					transmission_rounds_ssthres.push_back(*ssthres);
					transmission_rounds_window.push_back(*window);
					transmission_rounds.push_back(trans_round);
					trans_round++;
				}
			}
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
			if (dupAckIndex < dupAcks.size() && successAcks == dupAcks[dupAckIndex]) {
				successAcks = 0;
				dupAckIndex++;
				window_decrease(window, ssthres, miniWin, false);
			}
		}
		return true;
	}

	return false;
}

void SelectiveRepeatCCServer::window_decrease(unsigned int *window, unsigned int *ssthres,
		unsigned int *miniWin, bool timeout) {
	transmission_rounds_ssthres.push_back(*ssthres);
	transmission_rounds_window.push_back(*window);
	transmission_rounds.push_back(trans_round - 0.5);
	*ssthres = *window / 2;
	*miniWin = 0;
	if (timeout) {
		*window = 1;
	} else {
		*window = *ssthres;
	}
	transmission_rounds_ssthres.push_back(*ssthres);
	transmission_rounds_window.push_back(*window);
	transmission_rounds.push_back(trans_round);
	trans_round++;
	if (seq_no - 1 >= *window + base_seq_no) {
		for (unsigned int i = seq_no - 1; i >= *window + base_seq_no; i--) {
			if (data_sent.find(i) != data_sent.end()) {
				out_of_window_packets[i] = data_sent[i];
				seqnums_sent.erase(seqnums_sent.find(i));
				data_sent.erase(data_sent.find(i));
				sendOrder.erase(sendOrderIterators[i]);
				sendOrderIterators.erase(sendOrderIterators.find(i));
			}
		}
		seq_no = *window + base_seq_no;
	}
}
