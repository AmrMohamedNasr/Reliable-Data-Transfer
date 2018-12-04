/*
 * GoBackServer.h
 *
 *  Created on: Dec 4, 2018
 *      Author: amrnasr
 */

#ifndef SRC_WORKERS_GOBACKN_GOBACKSERVER_CPP_
#define SRC_WORKERS_GOBACKN_GOBACKSERVER_CPP_

#include "../serverWorker.h"

class GoBackServer : ServerWorker {
	private:
	// Implementation dependent.
	public:
		~GoBackServer();
		void send_message(DataFeeder dataFeeder, unsigned int seed, float loss_prob,
				int sendSocket, const struct sockaddr * clientAddr);
};

#endif
