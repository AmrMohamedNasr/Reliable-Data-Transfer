/*
 * GoBackClient.h
 *
 *  Created on: Dec 4, 2018
 *      Author: amrnasr
 */

#ifndef SRC_WORKERS_GOBACKN_GOBACKCLIENT_H_
#define SRC_WORKERS_GOBACKN_GOBACKCLIENT_H_


#include "../clientWorker.h"
#include "../web_models/packet.h"
#include "../web_models/packet_utils.h"
#include "../utils/socketUtils.h"
#include <sys/socket.h>

class GoBackClient : public ClientWorker {
	private:
	bool error;
    	bool timeout;
	uint32_t base_ack_no;
     	struct sockaddr_in src_addr;
     	
	public:
		// Use constructor to take all needed info from upper class.
		~GoBackClient();
		void recv_message(int socketFd, DataSink sink);
};


#endif /* SRC_WORKERS_GOBACKN_GOBACKCLIENT_H_ */
