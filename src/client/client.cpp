/*
 * client.cpp
 *
 *  Created on: Dec 1, 2018
 *      Author: michael
 */

#include "client.h"
#include "../parser/ArgumentParser.h"
#include <iostream>

using namespace std;

Client::Client(string path) {
	bool error;
	struct clientParameters params = readClientParameters(path, &error);
	if (error) {
		cout << "Invalid Parameter File" << endl;
		exit(1);
	}
	this->server_ip = params.inAdd;
	this->port = params.cliPort;
	this->server_port = params.servPort;
	this->recv_window = params.windowSize;
	this->filename = params.filename;
	print_client_parameters(&params);
}

void Client::start() {

}


