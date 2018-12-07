/*
 * StopWaitClientWorker.h
 *
 *  Created on: Dec 4, 2018
 *      Author: amrnasr
 */

#ifndef SRC_WORKERS_STOPWAIT_STOPWAITCLIENT_H_
#define SRC_WORKERS_STOPWAIT_STOPWAITCLIENT_H_

#include "../clientWorker.h"
#include "../../web_models/ack_packet.h"

class StopWaitClient :  public ClientWorker {
	private:
		struct sockaddr_in src_addr;
		bool error;
		bool time_out;
		struct ack_packet last_pack;
		bool sentOne;
	public:
		// Use constructor to take all needed info from upper class.
		~StopWaitClient();
		void recv_message(int socketFd, DataSink *sink, unsigned int window);
};



#endif /* SRC_WORKERS_STOPWAIT_STOPWAITCLIENT_H_ */
