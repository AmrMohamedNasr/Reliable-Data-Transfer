/*
 * clientWorker.h
 *
 *  Created on: Dec 4, 2018
 *      Author: amrnasr
 */

#ifndef SRC_SERVER_WORKERS_CLIENTWORKER_H_
#define SRC_SERVER_WORKERS_CLIENTWORKER_H_

#include "../../web_models/app_data.h"

class ClientWorker {
	public:
	    virtual ~ClientWorker(){}
	    virtual struct app_data * recv_message() = 0;
};


#endif /* SRC_SERVER_WORKERS_CLIENTWORKER_H_ */
