/*
 * selective_repeat.h
 *
 *  Created on: Dec 2, 2018
 *      Author: michael
 */

#ifndef SRC_RDT_METHODS_SELECTIVE_REPEAT_H_
#define SRC_RDT_METHODS_SELECTIVE_REPEAT_H_

#include "rdt.h"


class selective_repeat : public RDT {
	private:
		// list of datagrams and any other needed variable for implementation
	public:

		selective_repeat();
		selective_repeat(int n);
};



#endif /* SRC_RDT_METHODS_SELECTIVE_REPEAT_H_ */
