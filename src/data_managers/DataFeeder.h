/*
 * DataFeeder.h
 *
 *  Created on: Dec 4, 2018
 *      Author: amrnasr
 */

#ifndef SRC_DATA_MANAGERS_DATAFEEDER_H_
#define SRC_DATA_MANAGERS_DATAFEEDER_H_

#include "../web_models/packet.h"
#include <string>

using namespace std;

class DataFeeder {
	public:
		bool readFile(string path);
		bool hasNext();
		struct packet_core_data getNextDataSegment();
};



#endif /* SRC_DATA_MANAGERS_DATAFEEDER_H_ */
