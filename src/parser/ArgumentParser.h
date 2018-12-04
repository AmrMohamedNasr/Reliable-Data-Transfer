/*
 * ArgumentParser.h
 *
 *  Created on: Dec 4, 2018
 *      Author: amrnasr
 */

#ifndef SRC_PARSER_ARGUMENTPARSER_H_
#define SRC_PARSER_ARGUMENTPARSER_H_

#include <vector>
#include <string>

using namespace std;

struct clientParameters {
	struct in_addr * inAdd;
	int servPort;
	int cliPort;
	string filename;
	unsigned int windowSize;
};

struct serverParameters {
	int servPort;
	unsigned int windowSize;
	unsigned int seed;
	float lossProb;
};

vector<string> readLines(string path);
struct serverParameters readServerParameters(string path, bool *error);
struct clientParameters readClientParameters(string path, bool *error);

void print_server_parameters(struct serverParameters * params);
void print_client_parameters(struct clientParameters * params);
#endif /* SRC_PARSER_ARGUMENTPARSER_H_ */
