/*
 * serverWorker.h
 *
 *  Created on: Dec 4, 2018
 *      Author: amrnasr
 */

#ifndef SRC_SERVER_WORKERS_SERVERWORKER_H_
#define SRC_SERVER_WORKERS_SERVERWORKER_H_

#include "../data_managers/DataFeeder.h"
#include "../web_models/app_data.h"

class ServerWorker {
	public:
		virtual ~ServerWorker(){}
		virtual void send_message(DataFeeder dataFeeder, float loss_prob,
				int sendSocket, const struct sockaddr * clientAddr, unsigned int window) = 0;
};



#endif /* SRC_SERVER_WORKERS_SERVERWORKER_H_ */
