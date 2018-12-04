/*
 * app_data.cpp
 *
 *  Created on: Dec 4, 2018
 *      Author: amrnasr
 */

#include "app_data.h"
#include <cstdlib>
#include <cstring>
#include <algorithm>    // std::min

void deleteAppData(struct app_data * dat) {
	free(dat->data);
	delete dat;
}

vector<struct packet> breakAppMessage(struct app_data * data) {
	unsigned int rem = data->lenData;
	unsigned int sent = 0;
	vector<struct packet> pckts = vector<struct packet> ();
	while (rem > 0) {
		unsigned int minSize = std::min((int64_t)PCK_DATA_SIZE, (int64_t)rem);
		struct packet pack;
		memset(pack.data, 0, PCK_DATA_SIZE);
		memcpy(pack.data, data->data + sent, minSize);
		pack.len += minSize;
		pckts.push_back(pack);
		rem -= minSize;
		sent += minSize;
	}
	return pckts;
}

struct app_data * assembleAppMessage(vector<struct packet*> pckts) {
	struct app_data * appData = new struct app_data();
	unsigned int len = 0;
	for (unsigned int i = 0; i < pckts.size(); i++) {
		len += (pckts[i]->len - PCK_HEADER_SIZE);
	}
	appData->data = (char *)malloc(sizeof(char) * len);
	appData->lenData = len;
	size_t size = 0;
	for (unsigned int i = 0; i < pckts.size(); i++) {
		memcpy(appData->data + size, pckts[i]->data, pckts[i]->len - PCK_HEADER_SIZE);
		size += (pckts[i]->len - PCK_HEADER_SIZE);
	}
	return appData;
}
