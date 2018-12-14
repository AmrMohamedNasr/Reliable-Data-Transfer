/*
 * SelectiveRepeatServer.h
 *
 *  Created on: Dec 4, 2018
 *      Author: amrnasr
 */

#ifndef SRC_WORKERS_SELECTIVEREPEAT_SELECTIVEREPEATSERVER_H_
#define SRC_WORKERS_SELECTIVEREPEAT_SELECTIVEREPEATSERVER_H_

#include "../serverWorker.h"
#include <map>
#include <vector>
#include <set>
#include <netinet/in.h>
#include <list>
#include <unordered_map>
#include <unordered_set>

class SelectiveRepeatServer : public ServerWorker {
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
		~SelectiveRepeatServer();
		void send_message(DataFeeder * dataFeeder, float loss_prob,
				int sendSocket, const struct sockaddr * clientAddr, unsigned int window);
};


#endif /* SRC_WORKERS_SELECTIVEREPEAT_SELECTIVEREPEATSERVER_H_ */
