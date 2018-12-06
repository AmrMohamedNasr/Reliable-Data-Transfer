/*
 * server.cpp
 *
 *  Created on: Dec 1, 2018
 *      Author: michael
 */
#include "server.h"
#include "../parser/ArgumentParser.h"
#include "../definitions.h"
#include "../web_models/packet_utils.h"
#include "../utils/socketUtils.h"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include<sys/wait.h>
#include<sys/types.h>
#include <signal.h>
#include <sys/time.h>

#include <iostream>

using namespace std;

void my_handler(int s){
	   printf("Server closed...");
	   exit(0);
}


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
	struct sigaction sigIntHandler;
	sigIntHandler.sa_handler = my_handler;
	sigemptyset(&sigIntHandler.sa_mask);
	sigIntHandler.sa_flags = 0;
	sigaction(SIGINT, &sigIntHandler, NULL);

	int ntry = 0;
	int listenSocket;
	while((listenSocket = socket(AF_INET, SOCK_DGRAM, 0)) < 0 && ntry < RETRIES) {
		ntry++;
		cout << "Failed to open listen port. Retrying again for " << ntry << " time of " << RETRIES << " retries !" << endl;
	}
	if (listenSocket < 0) {
		perror("Socket creation");
		cout << "Could not open listen port. Ending program !" << endl;
		return;
	}
	struct sockaddr_in listenAddr;
	memset(&listenAddr, 0 , sizeof(sockaddr_in));
	listenAddr.sin_family = AF_INET;
	listenAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	listenAddr.sin_port = htons(this->port);
	ntry = 0;
	int success = -1;
	while((success = bind(listenSocket, (struct sockaddr *) &listenAddr, sizeof(listenAddr))) < 0 && ntry < RETRIES) {
		ntry++;
		cout << "Failed to bind listen port. Retrying again for " << ntry << " time of " << RETRIES << " retries !" << endl;
	}
	if (success < 0) {
		perror("bind");
		cout << "Could not bind listen port. Ending program !" << endl;
		close(listenSocket);
		return;
	}
	cout << "Listening on port : " << this->port << endl;
	struct sockaddr_in clientAddr;
	while (1) {
		memset(&clientAddr, 0, sizeof(struct sockaddr_in));
		struct packet pck = receive_packet(listenSocket,(struct sockaddr *)&clientAddr);
		pid_t pid = fork();
		if (pid == 0) {
			pid = getpid();
			srand(this->seed);
			cout << pid << " : Handling Request : " << inet_ntoa(clientAddr.sin_addr) << ":"
								<< ntohs(clientAddr.sin_port) << endl;
			int ntry = 0;
			int serverSocket;
			while((serverSocket = socket(AF_INET, SOCK_DGRAM, 0)) < 0 && ntry < RETRIES) {
				ntry++;
				cout << pid << " : Failed to open server child port. Retrying again for " << ntry << " time of " << RETRIES << " retries !" << endl;
			}
			if (serverSocket < 0) {
				perror("Socket creation");
				cout << pid << " : Could not open server child port. Ending program !" << endl;
				exit(1);
			}
			DataFeeder feeder = DataFeeder();
			char filePath[PCK_DATA_SIZE + 1];
			memset(filePath, 0, PCK_DATA_SIZE + 1);
			memcpy(filePath, pck.data, pck.len - PCK_HEADER_SIZE);
			if (!feeder.readFile(string(filePath))) {
				cout << pid << " : Invalid file (" << filePath << "): child closing" << endl;
				close(serverSocket);
				exit(1);
			}
			cout << pid << " : Sending file : " << filePath << endl;
			ServerWorker * worker = createServerWorker(type);
			struct timeval startTime;
			gettimeofday(&startTime, NULL);
			worker->send_message(&feeder,this->loss_prob, serverSocket,(sockaddr *)&clientAddr, this->slideWindow);
			struct timeval endTime;
			gettimeofday(&endTime, NULL);
			long int timeTaken = ((endTime.tv_sec - startTime.tv_sec)*1000000L
			           +endTime.tv_usec) - startTime.tv_usec;
			delete(worker);
			close(serverSocket);
			cout << pid << " : Finished operation" << endl;
			printf("%u transmission time :\n\tsec : %ld sec\n\tmsec : %ld msec\n\tMsec : %ld Msec\n",
				pid, (timeTaken / 1000000L), ((timeTaken / 1000L) % 1000L), (timeTaken % 1000L)
				);
			exit(0);
		} else if (pid < 0) {
			cout << "Failed to create child to handle request from : " << inet_ntoa(clientAddr.sin_addr) << ":"
					<< ntohs(clientAddr.sin_port) << endl;
		}
		signal(SIGCHLD,SIG_IGN);
	}
	close(listenSocket);
	cout << "Connection closed" << endl;
}

