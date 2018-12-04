/*
 * socketUtils.cpp
 *
 *  Created on: Dec 4, 2018
 *      Author: amrnasr
 */
#include "../web_models/packet.h"
#include "../web_models/ack_packet.h"
#include "../web_models/packet_utils.h"
#include <iostream>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
struct packet receive_packet(int sock, struct sockaddr *src_addr, bool *error, bool*time_out) {
	struct packet packet;
	memset(&packet, 0, sizeof(struct packet));
	// TODO time_out
	if (recvfrom(sock, &packet,
			sizeof(struct ack_packet),SOCK_DGRAM, src_addr, (socklen_t*) sizeof(*src_addr)) >= 0) {
		if (recv(sock, packet.data, packet.len, SOCK_DGRAM)>= 0) {
			if (!verifyChecksum(&packet)) {
				*error = true;
			}
			return packet;
		}
	}
	*error = true;
	return packet;
}

bool send_packet(int sock, struct sockaddr *dest_addr, struct packet * pack) {
	if (sendto(sock, pack, sizeof(pack), SOCK_DGRAM, dest_addr, sizeof(*dest_addr))>= 0) {
		return true;
	}
	return false;
}

bool hasData(int sock) {

}


