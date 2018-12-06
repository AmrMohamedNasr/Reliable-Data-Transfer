/*
 * serial.cpp
 *
 *  Created on: Dec 5, 2018
 *      Author: amrnasr
 */
#include "serial.h"
#include <cstring>

unsigned char * serialize_uint16(unsigned char *buffer, uint16_t value) {
	buffer[0] = value >> 8;
	buffer[1] = value;
	return buffer + 2;
}

unsigned char * serialize_uint32(unsigned char *buffer, uint32_t value) {
	buffer[0] = value >> 24;
	buffer[1] = value >> 16;
	buffer[2] = value >> 8;
	buffer[3] = value;
	return buffer + 4;
}



unsigned char * serialize_charbuffer(unsigned char *buffer, char * value, int num) {
	memcpy(buffer, value, num);
	return buffer + num;
}


unsigned int serialize_packet(unsigned char *buffer, struct packet *pack) {
	unsigned char *temp = buffer;
	temp = serialize_uint16(temp, pack->check_sum);
	temp = serialize_uint16(temp, pack->len);
	temp = serialize_uint32(temp, pack->seqno);
	temp = serialize_charbuffer(temp, pack->data, PCK_DATA_SIZE);
	return temp - buffer;
}

unsigned int serialize_ackpacket(unsigned char *buffer, struct ack_packet *pack) {
	unsigned char *temp = buffer;
	temp = serialize_uint16(temp, pack->cksum);
	temp = serialize_uint16(temp, pack->len);
	temp = serialize_uint32(temp, pack->ackno);
	return temp - buffer;
}

unsigned char * deserialize_uint16(unsigned char *buffer, uint16_t *value) {

	*value = ((uint16_t)buffer[0]) << 8;
	*value |= (uint16_t)buffer[1];
	return buffer + 2;
}

unsigned char * deserialize_uint32(unsigned char *buffer, uint32_t *value) {
	*value = ((uint32_t)buffer[0]) << 24;
	*value |= ((uint32_t)buffer[1]) << 16;
	*value |= ((uint32_t)buffer[2]) << 8;
	*value |= (uint32_t) buffer[3];
	return buffer + 4;
}
unsigned char * deserialize_charbuffer(unsigned char *buffer, char * value, int num) {
	memcpy(value, buffer, num);
	return buffer + num;
}

struct packet deserialize_packet(unsigned char *buffer) {
	unsigned char *temp = buffer;
	struct packet pack;
	memset(&pack, 0, sizeof(struct packet));
	temp = deserialize_uint16(temp, &(pack.check_sum));
	temp = deserialize_uint16(temp, &(pack.len));
	temp = deserialize_uint32(temp, &(pack.seqno));
	temp = deserialize_charbuffer(temp, pack.data, PCK_DATA_SIZE);
	return pack;
}

struct ack_packet deserialize_ackpacket(unsigned char *buffer) {
	unsigned char *temp = buffer;
	struct ack_packet pck;
	memset(&pck, 0, sizeof(struct ack_packet));
	temp = deserialize_uint16(temp, &(pck.cksum));
	temp = deserialize_uint16(temp, &(pck.len));
	temp = deserialize_uint32(temp, &(pck.ackno));
	return pck;
}
