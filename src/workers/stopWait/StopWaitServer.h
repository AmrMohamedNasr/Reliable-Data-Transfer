/*
 * StopWaitServer.h
 *
 *  Created on: Dec 4, 2018
 *      Author: michael
 */

#ifndef SRC_WORKERS_STOPWAIT_STOPWAITSERVER_H_
#define SRC_WORKERS_STOPWAIT_STOPWAITSERVER_H_

#include "../serverWorker.h"

class StopWaitServer : public ServerWorker {
	private:
	// Implementation dependent.
	public:
		~StopWaitServer();
		void send_message(DataFeeder dataFeeder, float loss_prob,
				int sendSocket, const struct sockaddr * clientAddr, unsigned int window);
};


#endif /* SRC_WORKERS_STOPWAIT_STOPWAITSERVER_H_ */
