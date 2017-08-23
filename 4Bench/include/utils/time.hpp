/*
 * time.hpp
 *
 *  Created on: Aug 18, 2017
 *      Author: galarraga
 */

#ifndef UTILS_TIME_HPP_
#define UTILS_TIME_HPP_

#include <string>
#include <chrono>

using namespace std;
using sc = chrono::steady_clock;

namespace fourbench {

sc::time_point time();

string formatElapsedTime(sc::time_point startUs, sc::time_point endUs);

}



#endif /* TIME_HPP_ */
