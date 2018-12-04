/*
 * app_data.h
 *
 *  Created on: Dec 4, 2018
 *      Author: amrnasr
 */

#ifndef SRC_WEB_MODELS_APP_DATA_H_
#define SRC_WEB_MODELS_APP_DATA_H_

#include <cstdlib>
#include <vector>

#include "packet.h"

using namespace std;

struct app_data {
	char *data;
	size_t lenData;
};

void deleteAppData(struct app_data * dat);

vector<struct packet*> breakAppMessage(struct app_data *);

struct app_data * assembleAppMessage(struct app_data *);

#endif /* SRC_WEB_MODELS_APP_DATA_H_ */
