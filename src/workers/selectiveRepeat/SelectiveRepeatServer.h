/*
 * SelectiveRepeatServer.h
 *
 *  Created on: Dec 4, 2018
 *      Author: amrnasr
 */

#ifndef SRC_WORKERS_SELECTIVEREPEAT_SELECTIVEREPEATSERVER_H_
#define SRC_WORKERS_SELECTIVEREPEAT_SELECTIVEREPEATSERVER_H_

#include "../serverWorker.h"

class SelectiveRepeatServer : public ServerWorker {
	private:
	// Implementation dependent.
	public:
		~SelectiveRepeatServer();
		void send_message(DataFeeder * dataFeeder, float loss_prob,
				int sendSocket, const struct sockaddr * clientAddr, unsigned int window);
};


#endif /* SRC_WORKERS_SELECTIVEREPEAT_SELECTIVEREPEATSERVER_H_ */
