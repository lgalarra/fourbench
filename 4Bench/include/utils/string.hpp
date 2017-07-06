/*
 * string.hpp
 *
 * String utilities
 *
 *  Created on: Jun 22, 2017
 *      Author: galarraga
 */

#ifndef UTILS_STRING_HPP_
#define UTILS_STRING_HPP_

#include <string>
#include <vector>

using namespace std;

namespace fourbench {

vector<string> split(const string& s, const string& delim);

string trim(const string& s);

string toLower(const string& s);

bool toBool(const string& s);

}

#endif /* UTILS_STRING_HPP_ */
