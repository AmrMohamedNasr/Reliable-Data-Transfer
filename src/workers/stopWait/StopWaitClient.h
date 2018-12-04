/*
 * StopWaitClientWorker.h
 *
 *  Created on: Dec 4, 2018
 *      Author: amrnasr
 */

#ifndef SRC_WORKERS_STOPWAIT_STOPWAITCLIENT_H_
#define SRC_WORKERS_STOPWAIT_STOPWAITCLIENT_H_

#include "../clientWorker.h"

#include <netinet/in.h>
#include <sys/socket.h>
class StopWaitClient :  public ClientWorker {
	private:
	struct sockaddr_in src_addr;
	bool error;
	bool time_out;
	public:
		// Use constructor to take all needed info from upper class.
		~StopWaitClient();
		void recv_message(int socketFd, DataSink sink);
};



#endif /* SRC_WORKERS_STOPWAIT_STOPWAITCLIENT_H_ */
