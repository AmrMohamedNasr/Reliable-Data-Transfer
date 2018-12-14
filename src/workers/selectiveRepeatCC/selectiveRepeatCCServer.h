/*
 * selectiveRepeatCCServer.h
 *
 *  Created on: Dec 7, 2018
 *      Author: amrnasr
 */

#ifndef SRC_WORKERS_SELECTIVEREPEATCC_SELECTIVEREPEATCCSERVER_H_
#define SRC_WORKERS_SELECTIVEREPEATCC_SELECTIVEREPEATCCSERVER_H_


#include "../serverWorker.h"
#include <map>
#include <vector>
#include <set>
#include <netinet/in.h>
#include <list>
#include <unordered_map>
#include <unordered_set>
class SelectiveRepeatCCServer : public ServerWorker {
	private:
		uint32_t base_seq_no;
		list<uint32_t> sendOrder;
		uint32_t seq_no;
		unordered_map<uint32_t, list<uint32_t>::iterator> sendOrderIterators;
		map<uint32_t, struct timeval> seqnums_sent;
		map<uint32_t, struct packet> data_sent;
		map<uint32_t, struct packet> out_of_window_packets;
		set<uint32_t> acks;
		vector<unsigned int> dupAcks;
		unsigned int dupAckIndex;
		unsigned int successAcks;
		struct sockaddr_in src_addr;
		float trans_round;
		bool error;
		bool time_out;
		bool updateTimers(int sendSocket, const struct sockaddr * clientAddr, float loss_prob,
				unsigned int *window, unsigned int *ssthres, unsigned int *miniWin);
		bool receive_ack(int sendSocket, unsigned int *window, unsigned int *ssthres, unsigned int *miniWin);
		void window_decrease(unsigned int *window, unsigned int *ssthres, unsigned int *miniWin, bool timeout);
	public:
		vector<unsigned int> transmission_rounds_window;
		vector<unsigned int> transmission_rounds_ssthres;
		vector<float> transmission_rounds;
		SelectiveRepeatCCServer();
		~SelectiveRepeatCCServer();
		void send_message(DataFeeder * dataFeeder, float loss_prob,
				int sendSocket, const struct sockaddr * clientAddr, unsigned int window);
};

void write_congestion_rounds(ServerWorker *worker);

#endif /* SRC_WORKERS_SELECTIVEREPEATCC_SELECTIVEREPEATCCSERVER_H_ */
