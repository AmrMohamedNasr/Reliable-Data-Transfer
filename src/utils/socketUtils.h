/*
 * socketUtils.h
 *
 *  Created on: Dec 4, 2018
 *      Author: amrnasr
 */

#ifndef SRC_UTILS_SOCKETUTILS_H_
#define SRC_UTILS_SOCKETUTILS_H_

#include "../web_models/packet.h"


struct packet receive_packet(int sock, struct sockaddr *src_addr, bool *error, bool*time_out);
bool send_packet(int sock, struct sockaddr *dest_addr, struct packet * pack);
bool hasData(int sock);


#endif /* SRC_UTILS_SOCKETUTILS_H_ */
