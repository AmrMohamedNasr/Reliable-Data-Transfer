/*
 * packet_utils.h
 *
 *  Created on: Dec 4, 2018
 *      Author: amrnasr
 */

#ifndef SRC_WEB_MODELS_PACKET_UTILS_H_
#define SRC_WEB_MODELS_PACKET_UTILS_H_

#include <stdint.h>
#include "packet.h"

struct packet create_data_packet(char * data, uint16_t size, uint32_t seq_num);
struct packet create_data_packet(struct packet_core_data * core_data, uint32_t seq_num);
struct ack_packet create_ack_packet(uint32_t seq_num);
struct packet_core_data extract_pure_data(struct packet * pack);
bool verifyChecksum(struct packet * pack);
bool verifyChecksumAck(struct ack_packet * pack);
uint16_t calculateChecksum(struct packet * pack);
uint16_t calculateChecksumAck(struct ack_packet * pack);
#endif /* SRC_WEB_MODELS_PACKET_UTILS_H_ */
