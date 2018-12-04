/*
 * DataSink.h
 *
 *  Created on: Dec 4, 2018
 *      Author: amrnasr
 */

#ifndef SRC_DATA_MANAGERS_DATASINK_H_
#define SRC_DATA_MANAGERS_DATASINK_H_

#include <string>
#include "../web_models/packet.h"

using namespace std;

class DataSink {
	private:
	public:
		void set_write_file(string path);
		void feed_next_data(packet_core_data *data);
		/**
		 * Returns currently buffered data if exists.
		 * Should only be used at higher levels.
		 */
		struct app_data * get_buff();
};



#endif /* SRC_DATA_MANAGERS_DATASINK_H_ */
