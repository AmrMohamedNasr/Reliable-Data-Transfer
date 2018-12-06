/*
 * packet_utils.cpp
 *
 *  Created on: Dec 4, 2018
 *      Author: amrnasr
 */

#include "packet_utils.h"
#include <cstring>
#include "ack_packet.h"

struct packet create_data_packet(const char * data, uint16_t size, uint32_t seq_num) {
	struct packet pck;
	memset(&pck, 0, sizeof(struct packet));
	pck.len = PCK_HEADER_SIZE + size;
	pck.seqno = seq_num;
	memcpy(pck.data, data, size);
	pck.check_sum = calculateChecksum(&pck);
	return pck;
}

struct packet create_data_packet(struct packet_core_data * core_data, uint32_t seq_num) {
	return create_data_packet(core_data->data, core_data->size, seq_num);
}

struct ack_packet create_ack_packet(uint32_t seq_num) {
	struct ack_packet pck;
	memset(&pck, 0, sizeof(struct ack_packet));
	pck.ackno = seq_num;
	pck.len = PCK_HEADER_SIZE;
	pck.cksum = calculateChecksumAck(&pck);
	return pck;
}

struct packet_core_data extract_pure_data(struct packet * pack) {
	struct packet_core_data pc_data;
	memset(&pc_data, 0, sizeof(struct packet_core_data));
	memcpy(pc_data.data, pack->data, pack->len - PCK_HEADER_SIZE);
	pc_data.size = pack->len - PCK_HEADER_SIZE;
	return pc_data;
}

bool verifyChecksum(struct packet * pack) {
	return pack->check_sum == calculateChecksum(pack);
}

bool verifyChecksumAck(struct ack_packet * pack) {
	return pack->cksum == calculateChecksumAck(pack);
}

uint16_t calculateChecksum(struct packet * pack) {
	uint32_t sum = (uint32_t)pack->len;
	sum += (pack->seqno >> 16);
	if (sum | 0x0100) {
		sum &= 0xFEFF;
		sum += 1;
	}
	sum += (pack->seqno & 0x00FF);
	if (sum | 0x0100) {
		sum &= 0xFEFF;
		sum += 1;
	}
	uint16_t *dum_ptr = (uint16_t *)pack->data;
	for (unsigned int i = 0; i < PCK_DATA_SIZE / 16; i++) {
		sum += (uint32_t)(dum_ptr[i]);
		if (sum | 0x0100) {
			sum &= 0xFEFF;
			sum += 1;
		}
	}
	return (uint16_t) sum;
}

uint16_t calculateChecksumAck(struct ack_packet * pack) {
	uint32_t sum = pack->len;
	sum += (pack->ackno >> 16);
	if (sum | 0x0100) {
		sum &= 0xFEFF;
		sum += 1;
	}
	sum += (pack->ackno & 0x00FF);
	if (sum | 0x0100) {
		sum &= 0xFEFF;
		sum += 1;
	}
	return (uint16_t) sum;
}
