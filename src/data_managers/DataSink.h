/*
 * DataSink.h
 *
 *  Created on: Dec 4, 2018
 *      Author: amrnasr
 */

#ifndef SRC_DATA_MANAGERS_DATASINK_H_
#define SRC_DATA_MANAGERS_DATASINK_H_

#include <string>
#include <vector>
#include "../web_models/packet.h"
#include "../file_system/file_handler.h"
using namespace std;

class DataSink {
	private:
		FileHandler file_handler;
		vector<struct packet> packets;
	public:
		DataSink();
		void set_write_file(string path);
		void feed_next_data(packet_core_data *data, uint32_t seq_no);
		/**
		 * Returns currently buffered data if exists.
		 * Should only be used at higher levels.
		 */
		struct app_data * get_buff();
};



#endif /* SRC_DATA_MANAGERS_DATASINK_H_ */
