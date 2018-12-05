/*
 * socketUtils.cpp
 *
 *  Created on: Dec 4, 2018
 *      Author: amrnasr
 */
#include "../web_models/packet.h"
#include "../web_models/ack_packet.h"
#include "../web_models/packet_utils.h"
#include "../definitions.h"

#include <iostream>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>

using namespace std;

struct packet receive_packet(int sock, struct sockaddr *src_addr, bool *error, bool*time_out) {
	struct packet packet;
	memset(&packet, 0, sizeof(struct packet));
	struct timeval tv;
	tv.tv_sec = TIMEOUT;
	tv.tv_usec = 0;
	setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);
	socklen_t addrlen = sizeof(struct sockaddr_in);
	if (recvfrom(sock, &packet,
			sizeof(struct ack_packet), 0, src_addr, &addrlen) > 0) {
		unsigned int rem = packet.len - PCK_HEADER_SIZE;
		if (rem == 0) {
			*error = false;
			*time_out = false;
		} else {
			if (recv(sock, packet.data, rem, 0)> 0) {
				*error = false;
				*time_out = false;
				return packet;
			} else {
				if ((errno != EAGAIN) && (errno != EWOULDBLOCK)) {
					*error = true;
					*time_out = true;
				} else {
					*error = true;
					*time_out = false;
				}
			}
		}
	} else {
		if ((errno != EAGAIN) && (errno != EWOULDBLOCK)) {
			*error = true;
			*time_out = true;
		} else {
			*error = true;
			*time_out = false;
		}
	}
	return packet;
}

struct packet receive_packet(int sock, struct sockaddr *src_addr) {
	struct packet packet;
	memset(&packet, 0, sizeof(struct packet));
	socklen_t addrlen = sizeof(struct sockaddr_in);
	if (recvfrom(sock, (void *)&packet,
			sizeof(struct ack_packet), 0, src_addr, &addrlen) > 0) {
		unsigned int rem = packet.len - PCK_HEADER_SIZE;
		if (rem != 0) {
			if (recv(sock, packet.data, rem, 0)> 0) {
				for (int i = 0; i < packet.len - PCK_HEADER_SIZE; i++) {
					cout << packet.data[i];
				}
				cout << endl;
				return packet;
			} else {
				perror("ERROR in listening socket : data reading ");
				exit(1);
			}
		}
		return packet;
	} else {
		perror("ERROR in listening socket : header reading ");
		exit(1);
	}
	return packet;
}

struct ack_packet receive_ack_packet(int sock, const struct sockaddr *dest_addr, bool *error, bool*time_out) {

}
struct ack_packet receive_ack_packet(int sock, const struct sockaddr *dest_addr) {

}
bool send_packet(int sock, const struct sockaddr *dest_addr, struct packet * pack) {
	if (sendto(sock, pack, pack->len, 0, dest_addr, sizeof(struct sockaddr_in))> 0) {
		return true;
	}
	return false;
}

bool hasData(int sock) {
	int count;
	ioctl(sock, FIONREAD, &count);
	return count > 0;
}




