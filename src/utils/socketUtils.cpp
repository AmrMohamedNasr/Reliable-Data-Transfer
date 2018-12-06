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
#include "serial.h"
#include <sys/time.h>

using namespace std;

struct packet receive_packet(int sock, struct sockaddr *src_addr, bool *error, bool*time_out,
		struct timeval tv) {
	struct packet packet;
	memset(&packet, 0, sizeof(struct packet));
	setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);
	socklen_t addrlen = sizeof(struct sockaddr_in);
	unsigned char buff[PCK_DATA_SIZE + PCK_HEADER_SIZE];
	if (recvfrom(sock, buff,
			PCK_DATA_SIZE + PCK_HEADER_SIZE, 0, src_addr, &addrlen) > 0) {
			return deserialize_packet(buff);
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
	unsigned char buff[PCK_DATA_SIZE + PCK_HEADER_SIZE];
	if (recvfrom(sock, buff,
			PCK_DATA_SIZE + PCK_HEADER_SIZE, 0, src_addr, &addrlen) > 0) {
		return deserialize_packet(buff);
	} else {
		perror("ERROR in listening socket : reading ");
		exit(1);
	}
	return packet;
}

struct ack_packet receive_ack_packet(int sock, struct sockaddr *src_addr, bool *error, bool*time_out,
		struct timeval tv) {
	struct ack_packet packet;
	memset(&packet, 0, sizeof(struct ack_packet));
	setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);
	socklen_t addrlen = sizeof(struct sockaddr_in);
	unsigned char buff[PCK_HEADER_SIZE];
	if (recvfrom(sock, buff,
			PCK_HEADER_SIZE, 0, src_addr, &addrlen) > 0) {
			return deserialize_ackpacket(buff);
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

bool send_ack_packet(int sock, const struct sockaddr *dest_addr, struct ack_packet * pack) {
	unsigned char buff[PCK_HEADER_SIZE];
	unsigned int size = serialize_ackpacket(buff, pack);
	if (sendto(sock, buff, size, 0, dest_addr, sizeof(struct sockaddr_in))> 0) {
		return true;
	}
	return false;
}

bool send_packet(int sock, const struct sockaddr *dest_addr, struct packet * pack) {
	unsigned char buff[PCK_DATA_SIZE + PCK_HEADER_SIZE];
	unsigned int size = serialize_packet(buff, pack);
	if (sendto(sock, buff, size, 0, dest_addr, sizeof(struct sockaddr_in))> 0) {
		return true;
	}
	return false;
}

bool hasData(int sock) {
	int count;
	ioctl(sock, FIONREAD, &count);
	return count > 0;
}

bool update_remaining_timeout(struct timeval * tv, struct timeval *old_time) {
	struct timeval newTime;
	gettimeofday(&newTime, NULL);
	double new_time_in_mill = (newTime.tv_sec) * 1000000.0 + (newTime.tv_usec);
	double old_time_in_mill = (old_time->tv_sec) * 1000000.0 + (old_time->tv_usec);
	double diff = new_time_in_mill - old_time_in_mill;
	double currentDiff = (tv->tv_sec) * 1000000.0 + (tv->tv_usec);
	*old_time = newTime;
	tv->tv_sec = tv->tv_sec - (diff / 1000000L);
	tv->tv_usec = tv->tv_usec - (((long)diff) % 1000000);
	return currentDiff <= diff;
}



