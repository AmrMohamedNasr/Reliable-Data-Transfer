/*
 * StopWaitServer.cpp
 *
 *  Created on: Dec 4, 2018
 *      Author: amrnasr
 */

#ifndef SRC_WORKERS_STOPWAIT_STOPWAITSERVER_CPP_
#define SRC_WORKERS_STOPWAIT_STOPWAITSERVER_CPP_

#include "../serverWorker.h"

class StopWaitServer : ServerWorker {
	private:
	// Implementation dependent.
	public:
		~StopWaitServer();
		void send_message(DataFeeder dataFeeder, unsigned int seed, float loss_prob,
				int sendSocket, const struct sockaddr * clientAddr);
};



#endif /* SRC_WORKERS_STOPWAIT_STOPWAITSERVER_CPP_ */
