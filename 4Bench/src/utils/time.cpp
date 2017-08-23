/*
 * time.cpp
 *
 *  Created on: Aug 18, 2017
 *      Author: galarraga
 */

#include <string>
#include <sstream>
#include <sys/time.h>
#include <chrono>
#include "../include/utils/time.hpp"

using namespace std;

using ms = chrono::microseconds;
using sc = chrono::steady_clock ;

namespace fourbench {

sc::time_point time() {
	return sc::now();
}

string formatElapsedTime(sc::time_point startUs, sc::time_point endUs) {
	auto delta = chrono::duration_cast<ms>(endUs - startUs).count();
	stringstream strstrm;
	double val;
	string unitTag;
	long oneMinute = 60 * 1e+06;
	long oneHour = 60 * oneMinute;
	if (delta < 1000) {
		val = delta;
		unitTag = " us";
	} else if (delta >= 1000 && delta < 1e+05) {
		val = delta / 1000;
		unitTag = " ms";
	} else if (delta >= 1e+05 && delta < oneMinute) {
		val = delta / 1e+06;
		unitTag = " s";
	} else if (delta >= oneMinute && delta < oneHour) {
		val = delta / oneMinute;
		unitTag = " h";
	}

	strstrm << val << unitTag;
	return strstrm.str();
}

}

