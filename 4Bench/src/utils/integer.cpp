/*
 * integer.cpp
 *
 *  Created on: Jul 14, 2017
 *      Author: galarraga
 */

#include <stdlib.h>

#include "../include/utils/integer.hpp"

namespace fourbench {

unsigned urand(unsigned lower, unsigned upper) {
	unsigned width = upper - lower;
	return lower + (rand() % width);
}

}
