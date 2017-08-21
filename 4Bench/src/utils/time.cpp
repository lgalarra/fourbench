/*
 * time.cpp
 *
 *  Created on: Aug 18, 2017
 *      Author: galarraga
 */

#include <sys/time.h>
#include "../include/utils/time.hpp"

namespace fourbench {

long timeMicroSeconds() {
	timeval tv;
	gettimeofday(&tv, 0);
	return tv.tv_usec;
}

}

