/*
 * FileParser.cpp
 *
 *  Created on: Jun 29, 2017
 *      Author: galarraga
 */

#include <iostream>
#include <fstream>
#include <vector>

#include "../include/parsing/FileParser.hpp"

namespace fourbench {
namespace parsing {

FileParser::FileParser(const vector<string>& filenames) : filenames(filenames),
		currentFileIdx(0), currentStream(nullptr) {
	init();
}

FileParser::~FileParser() {
	while (!streams.empty()) {
		vector<ifstream*>::iterator itr = streams.begin();
		ifstream* target = *itr;
		streams.erase(itr);
		delete target;
	}
}

void FileParser::init() {}

} /* namespace parsing */
} /* namespace fourbench */
