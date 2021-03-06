/*
 * clientWorker.h
 *
 *  Created on: Dec 4, 2018
 *      Author: amrnasr
 */

#ifndef SRC_SERVER_WORKERS_CLIENTWORKER_H_
#define SRC_SERVER_WORKERS_CLIENTWORKER_H_

#include "../web_models/app_data.h"
#include "../data_managers/DataSink.h"
#include <netinet/in.h>
#include <sys/socket.h>
class ClientWorker {
	public:
	    virtual ~ClientWorker(){}
	    virtual void recv_message(int socketFd, DataSink *sink, unsigned int window) = 0;
};


#endif /* SRC_SERVER_WORKERS_CLIENTWORKER_H_ */
