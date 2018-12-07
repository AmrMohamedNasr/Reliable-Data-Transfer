/*
 * WorkerFactory.cpp
 *
 *  Created on: Dec 4, 2018
 *      Author: amrnasr
 */

#include "WorkerFactory.h"

#include "goBackN/GoBackClient.h"
#include "goBackN/GoBackServer.h"
#include "selectiveRepeat/SelectiveRepeatClient.h"
#include "selectiveRepeat/SelectiveRepeatServer.h"
#include "stopWait/StopWaitClient.h"
#include "stopWait/StopWaitServer.h"
#include "selectiveRepeatCC/selectiveRepeatCCClient.h"
#include "selectiveRepeatCC/selectiveRepeatCCServer.h"

ClientWorker * createClientWorker(PROTO_TYPE type) {
	switch(type) {
		case PROTO_TYPE::GO_BACK_N:
			return new GoBackClient();
		case PROTO_TYPE::SELECTIVE_REPEAT:
			return new SelectiveRepeatClient();
		case PROTO_TYPE::WAIT_AND_STOP:
			return new StopWaitClient();
		case PROTO_TYPE::SELECTIVE_REPEAT_CC:
			return new SelectiveRepeatCCClient();
		default:
			return nullptr;
	}
}

ServerWorker * createServerWorker(PROTO_TYPE type) {
	switch(type) {
			case PROTO_TYPE::GO_BACK_N:
				return new GoBackServer();
			case PROTO_TYPE::SELECTIVE_REPEAT:
				return new SelectiveRepeatServer();
			case PROTO_TYPE::WAIT_AND_STOP:
				return new StopWaitServer();
			case PROTO_TYPE::SELECTIVE_REPEAT_CC:
						return new SelectiveRepeatCCServer();
			default:
				return nullptr;
		}
}

PROTO_TYPE obtainType(int id) {
	switch(id) {
		case 4:
			return PROTO_TYPE::SELECTIVE_REPEAT_CC;
		case 3:
			return PROTO_TYPE::GO_BACK_N;
		case 2:
			return PROTO_TYPE::SELECTIVE_REPEAT;
		case 1:
			return PROTO_TYPE::WAIT_AND_STOP;
		default:
			return PROTO_TYPE::WAIT_AND_STOP;
	}
}
string typeToString(PROTO_TYPE type) {
	switch(type) {
		case PROTO_TYPE::GO_BACK_N:
			return string("Go Back N");
		case PROTO_TYPE::SELECTIVE_REPEAT:
			return string("Selective Repeat");
		case PROTO_TYPE::WAIT_AND_STOP:
			return string("Wait & Stop");
		case PROTO_TYPE::SELECTIVE_REPEAT_CC:
			return string("Selective Repeat with Congestion Control");
		default:
			return string("Wait & Stop");
	}
}
