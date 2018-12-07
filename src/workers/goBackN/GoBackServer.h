/*
 * GoBackServer.h
 *
 *  Created on: Dec 4, 2018
 *      Author: amrnasr
 */

#ifndef SRC_WORKERS_GOBACKN_GOBACKSERVER_CPP_
#define SRC_WORKERS_GOBACKN_GOBACKSERVER_CPP_

#include "../serverWorker.h"
#include "../../web_models/packet.h"
#include "../../web_models/ack_packet.h"
#include "../../web_models/packet_utils.h"
#include "../../utils/socketUtils.h"
#include <sys/socket.h>
#include <netinet/in.h>

class GoBackServer : public ServerWorker {
	private:
	struct sockaddr_in src_addr;
	uint32_t seqno;
	uint32_t base_seq_no;
	bool error;
	bool timeout;
	struct ack_packet ack_packet;
	vector <struct packet> unacked_packet;
	bool receive_ack(int sendSocket, unsigned int window);
	public:
		~GoBackServer() {

		};
		void send_message(DataFeeder *dataFeeder, float loss_prob,
				int sendSocket, const struct sockaddr * clientAddr, unsigned int window);
};

#endif
