/*
 * file_reader.cpp
 *
 *  Created on: Nov 6, 2018
 *      Author: amrnasr
 */

#include "file_reader.h"
#include <string.h>

FileReader::FileReader() {
	this->ifs = nullptr;
}

void FileReader::set_file(string path) {
	if (ifs != nullptr) {
		delete ifs;
	}
	this->ifs = new ifstream(path);
}

size_t FileReader::read_chunk(int size, char * buff) {
	if (ifs == nullptr || !*(ifs)) {
		memset(buff, 0, size);
		return 0;
	}
	memset(buff, 0, size);
	ifs->read(buff, size);
	return ifs->gcount();
}

FileReader::~FileReader() {
	if (ifs != nullptr) {
		delete ifs;
	}
}
