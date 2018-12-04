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

ClientWorker * createClientWorker(PROTO_TYPE type) {
	switch(type) {
		case PROTO_TYPE::GO_BACK_N:
			return new GoBackClient();
		case PROTO_TYPE::SELECTIVE_REPEAT:
			return new SelectiveRepeatClient();
		case PROTO_TYPE::WAIT_AND_STOP:
			return new StopWaitClient();
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
			default:
				return nullptr;
		}
}


