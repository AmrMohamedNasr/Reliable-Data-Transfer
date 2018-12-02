/*
 * file_writer.h
 *
 *  Created on: Dec 2, 2018
 *      Author: michael
 */

#ifndef SRC_FILE_SYSTEM_FILE_WRITER_H_
#define SRC_FILE_SYSTEM_FILE_WRITER_H_


#include <string>
#include <fstream>
#include <streambuf>

using namespace std;

class FileWriter {
	private:
		ofstream ofs;
	public:
		void set_file(string path);
		bool write_chunk(char * chunk, streamsize size);
};



#endif /* SRC_FILE_SYSTEM_FILE_WRITER_H_ */
