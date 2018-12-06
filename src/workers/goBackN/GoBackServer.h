/*
 * GoBackServer.h
 *
 *  Created on: Dec 4, 2018
 *      Author: amrnasr
 */

#ifndef SRC_WORKERS_GOBACKN_GOBACKSERVER_CPP_
#define SRC_WORKERS_GOBACKN_GOBACKSERVER_CPP_

#include "../serverWorker.h"
#include "../web_models/packet.h"
#include "../web_models/packet_utils.h"
#include "../utils/socketUtils.h"
#include <sys/socket.h>
class GoBackServer : public ServerWorker {
	private:
	int seq_num = 1;
	int unacked_data_counter = 0;
	int window;
	bool error;
	bool timeout;
	struct ack_packet * acknoledgement;
	vector <struct packet*> unacked_packets;
	public:
		~GoBackServer();
		void send_message(DataFeeder dataFeeder, float loss_prob,
				int sendSocket, const struct sockaddr * clientAddr);
};

#endif
