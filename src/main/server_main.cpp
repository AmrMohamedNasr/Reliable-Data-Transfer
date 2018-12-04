/*
 * server_main.cpp
 *
 *  Created on: Dec 1, 2018
 *      Author: michael
 */
#include <iostream>
#include "../server/server.h"

using namespace std;

int main(int argc, char** argv) {
	string file;
	cout << "Enter File path to read server specification: " << endl;
	cin >> file;
	Server server(file);
	server.start();
	return 0;
}



