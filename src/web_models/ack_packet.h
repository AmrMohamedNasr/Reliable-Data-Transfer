/*
 * ack_packet.h
 *
 *  Created on: Dec 1, 2018
 *      Author: michael
 */

#ifndef SRC_WEB_MODELS_ACK_PACKET_H_
#define SRC_WEB_MODELS_ACK_PACKET_H_
#include <stdint.h>
class ack_packet{
	private:
		uint16_t cksum; /* Optional bonus part */
		uint16_t len;
		uint32_t ackno;
};




#endif /* SRC_WEB_MODELS_ACK_PACKET_H_ */
