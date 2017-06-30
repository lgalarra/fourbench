/*
 * FileParser.cpp
 *
 *  Created on: Jun 29, 2017
 *      Author: galarraga
 */

#include <iostream>
#include <fstream>

#include "../include/parsing/FileParser.hpp"

namespace fourbench {
namespace parsing {

FileParser::FileParser(const string& filename) : filename(filename) {
	stream = new ifstream(this->filename);
	init();
}

FileParser::~FileParser() {
	delete stream;
}

void FileParser::init() {}

} /* namespace parsing */
} /* namespace fourbench */
