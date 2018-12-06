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

class SelectiveRepeatServer : public ServerWorker {
	private:
		uint32_t base_seq_no;
		map<uint32_t, struct timeval> seqnums_sent;
		map<uint32_t, struct packet> data_sent;
		set<uint32_t> acks;
		struct sockaddr_in src_addr;
		bool error;
		bool time_out;
		int cycle;
		bool updateTimers(int sendSocket, const struct sockaddr * clientAddr);
		bool receive_ack(int sendSocket, unsigned int window);
	public:
		~SelectiveRepeatServer();
		void send_message(DataFeeder * dataFeeder, float loss_prob,
				int sendSocket, const struct sockaddr * clientAddr, unsigned int window);
};


#endif /* SRC_WORKERS_SELECTIVEREPEAT_SELECTIVEREPEATSERVER_H_ */
