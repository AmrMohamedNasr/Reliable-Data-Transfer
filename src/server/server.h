/*
 * server.h
 *
 *  Created on: Dec 1, 2018
 *      Author: michael
 */

#ifndef SRC_SERVER_SERVER_H_
#define SRC_SERVER_SERVER_H_

#include <stdio.h>      /* printf, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>
#include <string>

using namespace std;

class Server {
	private:
		int port;
		time_t seed;
		int slideWindow;
		float loss_prob;
	public :
		// Path to file having the server parameters.
		Server(string path);
		// adjust parameters according to arguments in server_main
		void start();
};




#endif /* SRC_SERVER_SERVER_H_ */
