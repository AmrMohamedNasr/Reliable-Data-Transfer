/*
 * server.cpp
 *
 *  Created on: Dec 1, 2018
 *      Author: michael
 */
#include "server.h"
#include "../parser/ArgumentParser.h"
#include <iostream>

using namespace std;

Server::Server(string path) {
	bool error;
	struct serverParameters params = readServerParameters(path, &error);
	if (error) {
		cout << "Invalid Parameter File" << endl;
		exit(1);
	}
	this->loss_prob = params.lossProb;
	this->port = params.servPort;
	this->slideWindow = params.windowSize;
	this->seed = params.seed;
	print_server_parameters(&params);
}

void Server::start(PROTO_TYPE type) {

}

