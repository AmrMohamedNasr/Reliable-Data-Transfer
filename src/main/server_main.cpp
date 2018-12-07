/*
 * server_main.cpp
 *
 *  Created on: Dec 1, 2018
 *      Author: michael
 */
#include <iostream>
#include "../server/server.h"
#include "../workers/WorkerFactory.h"

using namespace std;

int main(int argc, char** argv) {
	string file;
	cout << "Enter File path to read server specification: " << endl;
	cin >> file;
	Server server(file);
	cout << "Enter wanted rdt protocol number : " << endl;
	cout << "1. Stop & wait" << endl;
	cout << "2. Selective Repeat" << endl;
	cout << "3. Go Back N" << endl;
	cout << "4. Selective Repeat with Congestion Control" << endl;
	int id;
	cin >> id;
	if (id < 1 || id > 4) {
		cout << "Invalid rdt protocol number !" << endl;
		exit(1);
	}
	PROTO_TYPE type = obtainType(id);
	cout << "Protocol chosen is : " << typeToString(type) << endl;
	cout << endl;
	server.start(type);
	return 0;
}



