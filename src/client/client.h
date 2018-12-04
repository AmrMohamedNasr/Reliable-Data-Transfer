/*
 * client.h
 *
 *  Created on: Dec 1, 2018
 *      Author: michael
 */

#ifndef SRC_CLIENT_CLIENT_H_
#define SRC_CLIENT_CLIENT_H_

#include <string>

using namespace std;

class Client {
	private :
		int server_port;
		int port;
		unsigned int recv_window;
		struct in_addr * server_ip;
		string filename;
	public :
		// String containing the path to the file having client parameters.
		Client(string path);
		// adjust parameters according to arguments in server_main
		void start();
};



#endif /* SRC_CLIENT_CLIENT_H_ */
