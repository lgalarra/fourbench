/*
 * Collection of utility functions.
 * utils.cpp
 *
 *  Created on: Jun 22, 2017
 *      Author: galarraga
 */

#include <algorithm>
#include <functional>
#include <cctype>
#include <locale>
#include <vector>
#include <set>
#include <iostream>
#include <fstream>

#include "../include/utils/string.hpp"

using namespace std;

namespace fourbench {

vector<string> split(const string& s, const string& delim) {
	vector<string> output;
	string data = s;
	size_t pos = string::npos; // size_t to avoid improbable overflow
	do {
		pos = data.find(delim);
		string part = data.substr(0, pos);
		output.push_back(part);
		if (string::npos != pos)
			data = data.substr(pos + delim.size());
	} while (string::npos != pos);

	return output;
}


string trim(const string &s) {
   auto wsfront = find_if_not(s.begin(),s.end(),[](int c){ return std::isspace(c);});
   auto wsback = find_if_not(s.rbegin(),s.rend(), [](int c){return std::isspace(c);}).base();
   return (wsback <= wsfront ? std::string() : std::string(wsfront,wsback) );
}


}
