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
struct packet receive_packet(int sock, struct sockaddr *src_addr);
struct ack_packet receive_ack_packet(int sock, const struct sockaddr *dest_addr, bool *error, bool*time_out);
struct ack_packet receive_ack_packet(int sock, const struct sockaddr *dest_addr);
bool send_packet(int sock, const struct sockaddr *dest_addr, struct packet * pack);
bool send_ack_packet(int sock, struct sockaddr *dest_addr, struct ack_packet * pack);
bool hasData(int sock);


#endif /* SRC_UTILS_SOCKETUTILS_H_ */
