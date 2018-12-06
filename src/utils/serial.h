/*
 * serial.h
 *
 *  Created on: Dec 5, 2018
 *      Author: amrnasr
 */

#ifndef SRC_UTILS_SERIAL_H_
#define SRC_UTILS_SERIAL_H_

#include <cstdint>
#include "../web_models/packet.h"
#include "../web_models/ack_packet.h"

unsigned char * serialize_uint16(unsigned char *buffer, uint16_t value);
unsigned char * serialize_uint32(unsigned char *buffer, uint32_t value);
unsigned char * serialize_charbuffer(unsigned char *buffer, char * value, int num);

unsigned int serialize_packet(unsigned char *buffer, struct packet *pack);
unsigned int serialize_ackpacket(unsigned char *buffer, struct ack_packet *pack);

unsigned char * deserialize_uint16(unsigned char *buffer, uint16_t *value);
unsigned char * deserialize_uint32(unsigned char *buffer, uint32_t *value);
unsigned char * deserialize_charbuffer(unsigned char *buffer, char * value, int num);

struct packet deserialize_packet(unsigned char *buffer);
struct ack_packet deserialize_ackpacket(unsigned char *buffer);

#endif /* SRC_UTILS_SERIAL_H_ */
