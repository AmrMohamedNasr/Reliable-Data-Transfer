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
#include <queue>
using namespace std;
class SelectiveRepeatClient : public ClientWorker {
	private:
		uint32_t base_ack_no;
		map<uint32_t, struct packet_core_data> data_received;
		struct sockaddr_in src_addr;
		bool error;
		bool time_out;
		void handleWindow(DataSink *sink);
	public:
		// Use constructor to take all needed info from upper class.
		~SelectiveRepeatClient();
		void recv_message(int socketFd, DataSink *sink, unsigned int window);
};


#endif /* SRC_WORKERS_SELECTIVEREPEAT_SELECTIVEREPEATCLIENT_H_ */
