/*
 * GoBackClient.h
 *
 *  Created on: Dec 4, 2018
 *      Author: amrnasr
 */

#ifndef SRC_WORKERS_GOBACKN_GOBACKCLIENT_H_
#define SRC_WORKERS_GOBACKN_GOBACKCLIENT_H_


#include "../clientWorker.h"

class GoBackClient : public ClientWorker {
	private:
	bool error;
    	bool timeout;
     	struct packet * recvPacket;
     	struct sockaddr * src_addr;
     	struct packet_core_data * corePacket;
	public:
		// Use constructor to take all needed info from upper class.
		~GoBackClient();
		void recv_message(int socketFd, DataSink *sink, unsigned int window);
};


#endif /* SRC_WORKERS_GOBACKN_GOBACKCLIENT_H_ */
