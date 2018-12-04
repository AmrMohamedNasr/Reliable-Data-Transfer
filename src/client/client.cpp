/*
 * client.cpp
 *
 *  Created on: Dec 1, 2018
 *      Author: michael
 */

#include "client.h"
#include "../parser/ArgumentParser.h"
#include "../web_models/packet_utils.h"
#include "../utils/socketUtils.h"

#include <iostream>
#include <cstring>

#define RETRIES	3
#define TIMEOUT	5
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

void Client::start(PROTO_TYPE type) {
	int ntry = 0;
	int clientSocket;
	while((clientSocket = socket(AF_INET, SOCK_DGRAM, 0)) < 0 && ntry < RETRIES) {
		ntry++;
		cout << "Failed to open client port. Retrying again for " << ntry << " time of " << RETRIES << " retries !" << endl;
	}
	if (clientSocket < 0) {
		perror("Socket creation");
		cout << "Could not open client port. Ending program !" << endl;
		return;
	}
	struct sockaddr_in serverAddr;
	memset(&serverAddr, 0 , sizeof(sockaddr_in));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = this->server_ip->s_addr;
	serverAddr.sin_port = htons(this->server_port);
	struct sockaddr_in clientAddr;
	memset(&clientAddr, 0 , sizeof(sockaddr_in));
	clientAddr.sin_family = AF_INET;
	clientAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	clientAddr.sin_port = htons(this->port);
	ntry = 0;
	int success = -1;
	while((success = bind(clientSocket, (struct sockaddr *) &clientAddr, sizeof(clientAddr))) < 0 && ntry < RETRIES) {
		ntry++;
		cout << "Failed to bind client port. Retrying again for " << ntry << " time of " << RETRIES << " retries !" << endl;
	}
	if (success < 0) {
		perror("bind");
		cout << "Could not bind client port. Ending program !" << endl;
		return;
	}
	struct packet pack = create_data_packet(this->filename.c_str(), (uint16_t)this->filename.size(), 0);
	bool error = true;
	struct timeval tv;
	tv.tv_sec = TIMEOUT;
	tv.tv_usec = 0;
	setsockopt(clientSocket, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);
	ntry = 0;
	while (error && ntry < RETRIES) {
		int ntrys = 0;
		while ((error = !send_packet(clientSocket, (struct sockaddr *) &serverAddr, &pack)) && ntrys < RETRIES) {
			ntrys++;
			cout << "Failed to send request. Retrying again for " << ntrys << " time of " << RETRIES << " retries !" << endl;
		}
		if (error) {
			perror("send");
			cout << "Could not send request. Ending program !" << endl;
			return;
		}
		ntry++;
		char c;
		if (recv(clientSocket, &c, 1, MSG_PEEK) == -1) {
			if ((errno != EAGAIN) && (errno != EWOULDBLOCK)) {
				cout << "Response timeout, retransmitting. Retrying again for " << ntry << " time of " << RETRIES << " retries !" << endl;
			} else {
				cout << "Failed to receive response. Retrying again for " << ntry << " time of " << RETRIES << " retries !" << endl;
			}
			error = true;
		} else {
			error = false;
		}
	}
	if (error) {
		perror("Receiving initial response :");
		cout << "Could not establish transmission with server. Ending program !" << endl;
		exit(1);
	}
	DataSink sink = DataSink();
	//sink.set_write_file(this->filename);
	sink.set_write_file("./ClientRecievedFile.zip");
	ClientWorker *worker = createClientWorker(type);
	worker->recv_message(clientSocket, sink, this->recv_window);
	delete worker;
}


