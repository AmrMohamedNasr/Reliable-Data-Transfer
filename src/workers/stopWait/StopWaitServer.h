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
		bool error;
		bool time_out;
		bool resend_packet(int sendSocket, const struct sockaddr * clientAddr,
				struct packet packet, uint32_t *seq_no, struct timeval tv, struct timeval sendTime);
	public:
		~StopWaitServer();
		void send_message(DataFeeder *dataFeeder, float loss_prob,
				int sendSocket, const struct sockaddr * clientAddr, unsigned int window);
};


#endif /* SRC_WORKERS_STOPWAIT_STOPWAITSERVER_H_ */
