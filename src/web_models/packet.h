/*
 * packet.h
 *
 *  Created on: Dec 1, 2018
 *      Author: michael
 */
#ifndef SRC_WEB_MODELS_PACKET_H_
#define SRC_WEB_MODELS_PACKET_H_

#include <iostream>
#include <stdlib.h>
#include <stdint.h>
#include <string>

using namespace std;

class packet {

	private:
		uint16_t check_sum;
		uint16_t len;
		uint32_t seqno;
		char data[500];
	public:



};




#endif /* SRC_WEB_MODELS_PACKET_H_ */
