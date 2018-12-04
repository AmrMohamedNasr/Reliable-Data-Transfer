/*
 * DataFeeder.cpp
 *
 *  Created on: Dec 4, 2018
 *      Author: amrnasr
 */
#include "DataFeeder.h"
#include <string.h>


bool DataFeeder::readFile(string path) {
	if (file_handler.check_file(path)) {
		file_handler.set_read_file(path);
		remain_size = file_handler.get_file_size(path);
		return true;
	}
	return false;
}

bool DataFeeder::hasNext() {
	return remain_size > 0;
}

struct packet_core_data DataFeeder::getNextDataSegment() {
	struct packet_core_data core_data;
	memset(&core_data, 0, sizeof(struct packet_core_data));
	if (remain_size > sizeof(char[500])) {
		file_handler.read_chunk(sizeof(char[500]), core_data.data);
		remain_size = remain_size - sizeof(char[500]);
		core_data.size = sizeof(char[500]);
	} else {
		file_handler.read_chunk(remain_size, core_data.data);
		core_data.size = remain_size;
		remain_size = 0;
	}
	return core_data;

}
