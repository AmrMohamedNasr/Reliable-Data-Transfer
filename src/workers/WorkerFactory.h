/*
 * WorkerFactory.h
 *
 *  Created on: Dec 4, 2018
 *      Author: amrnasr
 */

#ifndef SRC_WORKERS_WORKERFACTORY_H_
#define SRC_WORKERS_WORKERFACTORY_H_

#include "clientWorker.h"
#include "serverWorker.h"

enum class PROTO_TYPE {WAIT_AND_STOP, SELECTIVE_REPEAT, GO_BACK_N};

ClientWorker * createClientWorker(PROTO_TYPE type);
ServerWorker * createServerWorker(PROTO_TYPE type);

#endif /* SRC_WORKERS_WORKERFACTORY_H_ */
