/*
 * SelectiveRepeatClient.h
 *
 *  Created on: Dec 4, 2018
 *      Author: amrnasr
 */

#ifndef SRC_WORKERS_SELECTIVEREPEAT_SELECTIVEREPEATCLIENT_H_
#define SRC_WORKERS_SELECTIVEREPEAT_SELECTIVEREPEATCLIENT_H_



#include "../clientWorker.h"

class SelectiveRepeatClient : public ClientWorker {
	private:
	//Implemention Dependant.
	public:
		// Use constructor to take all needed info from upper class.
		~SelectiveRepeatClient();
		void recv_message(int socketFd, DataSink sink);
};


#endif /* SRC_WORKERS_SELECTIVEREPEAT_SELECTIVEREPEATCLIENT_H_ */
