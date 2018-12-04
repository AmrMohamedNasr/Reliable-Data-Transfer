/*
 * DataSink.cpp
 *
 *  Created on: Dec 4, 2018
 *      Author: amrnasr
 */
#include "DataSink.h"
#include <iostream>
#include <unistd.h>
#include <string.h>
#include "../web_models/packet_utils.h"
#include "../web_models/app_data.h"

DataSink::DataSink() {
	file_handler = FileHandler();
	packets = vector<struct packet>();
}

void DataSink::set_write_file(string path) {
	file_handler.set_write_file(path);
}

void DataSink::feed_next_data(packet_core_data *data, uint32_t seq_no) {
	file_handler.write_chunk(data->data, data->size);
	packets.push_back(create_data_packet(data, seq_no));
}

struct app_data * DataSink:: get_buff() {
	vector<struct packet *> pckts = vector<struct packet *>();
	for (unsigned i = 0; i < packets.size(); i++) {
		pckts.push_back(&packets[i]);
	}
	return assembleAppMessage(pckts);
}


