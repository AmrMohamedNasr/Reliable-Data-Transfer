/*
 * serverWorker.h
 *
 *  Created on: Dec 4, 2018
 *      Author: amrnasr
 */

#ifndef SRC_SERVER_WORKERS_SERVERWORKER_H_
#define SRC_SERVER_WORKERS_SERVERWORKER_H_

#include "../../web_models/app_data.h"

class ServerWorker {
	public:
		virtual ~ServerWorker(){}
		virtual void send_message(struct app_data * data) = 0;
};



#endif /* SRC_SERVER_WORKERS_SERVERWORKER_H_ */
