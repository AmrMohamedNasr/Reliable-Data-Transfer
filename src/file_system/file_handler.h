/*
 * file_handler.h
 *
 *  Created on: Dec 2, 2018
 *      Author: michael
 */

#ifndef SRC_FILE_SYSTEM_FILE_HANDLER_H_
#define SRC_FILE_SYSTEM_FILE_HANDLER_H_

#include <string>
#include "file_reader.h"
#include "file_writer.h"

using namespace std;

class FileHandler {
	private:
		FileReader reader;
		FileWriter writer;
	public:
		FileHandler();
		bool check_file(string path);
		size_t get_file_size(string path);
		void set_read_file(string path);
		void set_write_file(string path);
		size_t read_chunk(int size, char * buff);
		bool write_chunk(char * chunk, streamsize size);
};

#endif /* SRC_FILE_SYSTEM_FILE_HANDLER_H_ */
