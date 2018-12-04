/*
 * packet.h
 *
 *  Created on: Dec 1, 2018
 *      Author: michael
 */
#ifndef SRC_WEB_MODELS_PACKET_H_
#define SRC_WEB_MODELS_PACKET_H_

#include <stdint.h>

#define	PCK_DATA_SIZE	512
#define PCK_HEADER_SIZE	64

struct packet {
		uint16_t check_sum;
		uint16_t len;
		uint32_t seqno;
		char data[PCK_DATA_SIZE];
};

struct packet_core_data {
	uint16_t size;
	char data[PCK_DATA_SIZE];
};


#endif /* SRC_WEB_MODELS_PACKET_H_ */
