/*
 * SelectiveRepeatClient.h
 *
 *  Created on: Dec 4, 2018
 *      Author: amrnasr
 */

#ifndef SRC_WORKERS_SELECTIVEREPEAT_SELECTIVEREPEATCLIENT_H_
#define SRC_WORKERS_SELECTIVEREPEAT_SELECTIVEREPEATCLIENT_H_



#include "../clientWorker.h"
#include <map>
#include <vector>
class SelectiveRepeatClient : public ClientWorker {
	private:
		uint32_t base_seq_no;
		vector<pair<uint32_t, uint16_t>> wnd;
		struct sockaddr_in src_addr;
		bool error;
		bool time_out;
	public:
		// Use constructor to take all needed info from upper class.
		~SelectiveRepeatClient();
		void recv_message(int socketFd, DataSink sink);
};


#endif /* SRC_WORKERS_SELECTIVEREPEAT_SELECTIVEREPEATCLIENT_H_ */
