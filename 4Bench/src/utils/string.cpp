/*
 * Collection of utility functions.
 * utils.cpp
 *
 *  Created on: Jun 22, 2017
 *      Author: galarraga
 */

#include <vector>

#include "../include/utils/string.hpp"

using namespace std;

namespace fourbench {

vector<string> split(const string& s, const string& delim) {
	vector<string> output;
	string data = s;
	size_t pos = string::npos; // size_t to avoid improbable overflow
	do {
		pos = data.find(delim);
		output.push_back(data.substr(0, pos));
		if (string::npos != pos)
			data = data.substr(pos + delim.size());
	} while (string::npos != pos);

	return output;
}

}
