/*
 * datagram.h
 *
 *  Created on: Dec 1, 2018
 *      Author: michael
 */

#ifndef SRC_WEB_MODELS_DATAGRAM_H_
#define SRC_WEB_MODELS_DATAGRAM_H_
#include "packet.h"
#include "ack_packet.h"
class datagram {
	private:
		struct packet packet;
		struct ack_packet ack_packet;

	public:
		datagram(uint16_t len);

};



#endif /* SRC_WEB_MODELS_DATAGRAM_H_ */
