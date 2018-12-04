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
	int windowSize;
};

struct serverParameters {
	int servPort;
	int windowSize;
	time_t seed;
	float lossProb;
};

vector<string> readLines(string path);
struct serverParameters readServerParameters(string path);
struct clientParameters readClientParameters(string path);


#endif /* SRC_PARSER_ARGUMENTPARSER_H_ */
