/*
 * ArgumentParser.cpp
 *
 *  Created on: Dec 4, 2018
 *      Author: amrnasr
 */
#include "ArgumentParser.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define DELIM "."

#include <algorithm>
#include <cctype>
#include <locale>

// trim from start (in place)
static inline void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
        return !std::isspace(ch);
    }));
}

// trim from end (in place)
static inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

// trim from both ends (in place)
static inline void trim(std::string &s) {
    ltrim(s);
    rtrim(s);
}

// trim from both ends (copying)
static inline std::string trim_copy(std::string s) {
    trim(s);
    return s;
}

/* return 1 if string contain only digits, else return 0 */
int valid_digit(const char *ip_str)
{
    while (*ip_str) {
        if (*ip_str >= '0' && *ip_str <= '9')
            ++ip_str;
        else
            return 0;
    }
    return 1;
}

/* return 1 if IP string is valid, else return 0 */
int is_valid_ip(const char *o_ip_str, int size)
{
    int num, dots = 0;
    char *ptr;
    char ip_str[size+1];
    memcpy(ip_str, o_ip_str, size);
    ip_str[size] = '\0';
    if (ip_str == NULL)
        return 0;
    ptr = strtok(ip_str, DELIM);

    if (ptr == NULL)
        return 0;

    while (ptr) {

        /* after parsing string, it must contain only digits */
        if (!valid_digit(ptr))
            return 0;

        num = atoi(ptr);

        /* check for valid IP */
        if (num >= 0 && num <= 255) {
            /* parse remaining string */
            ptr = strtok(NULL, DELIM);
            if (ptr != NULL)
                ++dots;
        } else
            return 0;
    }

    /* valid IP string must contain 3 dots */
    if (dots != 3)
        return 0;
    return 1;
}

bool isFloat( string myString ) {
    std::istringstream iss(myString);
    float f;
    iss >> noskipws >> f;
    return iss.eof() && !iss.fail();
}

vector<string> readLines(string path) {
	vector<string> lines = vector<string>();
	ifstream in(path);
	if (in.is_open()) {
		string line;
		while (getline(in, line)) {
			lines.push_back(line);
		}
	}
	return lines;
}
struct serverParameters readServerParameters(string path, bool *error) {
	vector<string> lines = readLines(path);
	struct serverParameters params;
	*error = true;
	if (lines.size() == 4) {
		if (valid_digit(lines[0].c_str()) && valid_digit(lines[1].c_str()) && valid_digit(lines[2].c_str()) &&
				isFloat(lines[3])) {
			params.servPort = stoi(lines[0]);
			params.windowSize = stoul(lines[1]);
			params.seed = stoul(lines[2]);
			params.lossProb = stof(lines[3]);
			*error = params.lossProb < 0 || params.lossProb > 1;
		}
	}
	return params;
}
struct clientParameters readClientParameters(string path, bool *error) {
	vector<string> lines = readLines(path);
	struct clientParameters params;
	*error = true;
	if (lines.size() == 5) {
		if (valid_digit(lines[1].c_str()) && valid_digit(lines[2].c_str())
				&& is_valid_ip(lines[0].c_str(), lines[0].size()) && valid_digit(lines[4].c_str())) {
			hostent * record;
			record = gethostbyname(lines[0].c_str());
			params.inAdd = (struct in_addr *)record->h_addr_list[0];
			params.servPort = stoi(lines[1]);
			params.cliPort = stoi(lines[2]);
			params.filename = trim_copy(lines[3]);
			params.windowSize = stoul(lines[4]);
			*error = false;
		}
	}
	return params;
}

void print_server_parameters(struct serverParameters * params) {
	cout << "Server Port : " << params->servPort << endl;
	cout << "Server Sending Window : " << params->windowSize << endl;
	cout << "Server Seed : " << params->seed << endl;
	cout << "Server Loss Probability : " << params->lossProb << endl;
}

void print_client_parameters(struct clientParameters * params) {
	cout << "Server IP : " << inet_ntoa(*params->inAdd) << endl;
	cout << "Server Port : " << params->servPort << endl;
	cout << "Client Port : " << params->cliPort << endl;
	cout << "Client Receiving Window : " << params->windowSize << endl;
	cout << "File requested : " << params->filename << endl;
}
