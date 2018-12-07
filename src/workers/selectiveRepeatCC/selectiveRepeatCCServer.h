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
class SelectiveRepeatCCServer : public ServerWorker {
	private:
		uint32_t base_seq_no;
		list<uint32_t> sendOrder;
		unordered_map<uint32_t, list<uint32_t>::iterator> sendOrderIterators;
		map<uint32_t, struct timeval> seqnums_sent;
		map<uint32_t, struct packet> data_sent;
		set<uint32_t> acks;
		struct sockaddr_in src_addr;
		bool error;
		bool time_out;
		int cycle;
		bool updateTimers(int sendSocket, const struct sockaddr * clientAddr, float loss_prob);
		bool receive_ack(int sendSocket, unsigned int window);
	public:
		~SelectiveRepeatCCServer();
		void send_message(DataFeeder * dataFeeder, float loss_prob,
				int sendSocket, const struct sockaddr * clientAddr, unsigned int window);
};


#endif /* SRC_WORKERS_SELECTIVEREPEATCC_SELECTIVEREPEATCCSERVER_H_ */
