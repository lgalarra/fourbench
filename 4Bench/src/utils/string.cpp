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
#include <list>
#include <set>
#include <map>
#include <iostream>
#include <sstream>
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

vector<string> splitOnce(const string& s, const string& delim) {
	vector<string> output;
	string data = s;
	size_t pos = data.find(delim);
	string part = data.substr(0, pos);
	output.push_back(part);
	if (string::npos != pos) {
		data = data.substr(pos + delim.size());
		if (!data.empty())
			output.push_back(data);
	}

	return output;
}


string trim(const string &s) {
   auto wsfront = find_if_not(s.begin(),s.end(),[](int c){ return std::isblank(c) || c == '\n';});
   auto wsback = find_if_not(s.rbegin(),s.rend(), [](int c){return std::isblank(c) || c == '\n';}).base();
   return (wsback <= wsfront ? std::string() : std::string(wsfront,wsback) );
}

string trimIRI(const string& s) {
	auto start = s.begin();
	auto end = s.end();

	if (s.size() < 2)
		return s;

	if (*(start) == '<')
		++start;

	if (*(end - 1) == '>') {
		end = end - 1;
	}

	return std::string(start, end);

}

string toLower(const string& s) {
	string destination = s;
	transform(s.begin(), s.end(), destination.begin(), ::tolower);
	return destination;
}

bool toBool(const string& s) {
	static map<string, bool> str2bool = {{"true", true},
			{"1", true},
			{"false", false},
			{"0", false}};

	string lows = toLower(s);
	auto itr = str2bool.find(lows);
	if (itr == str2bool.end()) {
		return false;
	} else {
		return itr->second;
	}
}

string concat(const list<string>& strings) {
	stringstream sstrm;
	for (auto itr = strings.begin(); itr != strings.end(); ++itr) {
		sstrm << *itr;
	}

	return sstrm.str();
}

string wikify(const string& s) {
	stringstream sstrm;
	static vector<char> specialChars = {' ', '{', '}', '(', ')', '&'};

	for (auto it = s.cbegin(); it != s.cend(); ++it) {
		if (find(specialChars.cbegin(), specialChars.cend(), *it) == specialChars.cend()) {
			sstrm << *it;
		}
	}

	return sstrm.str();
}

}
