/*
 * FileParser.hpp
 *
 *  Created on: Jun 29, 2017
 *      Author: galarraga
 */

#ifndef PARSING_FILEPARSER_HPP_
#define PARSING_FILEPARSER_HPP_

#include <string>
#include <iostream>
#include <fstream>

#include "Triple.hpp"

using namespace std;

namespace fourbench {
namespace parsing {

class FileParser {
protected:
	string filename;
	ifstream* stream;
	virtual void init();
public:
	virtual Triple* next() = 0;
	virtual unsigned getNumberOfTriples() const = 0;
	FileParser(const string& filename);
	virtual ~FileParser();
};

} /* namespace parsing */
} /* namespace fourbench */

#endif /* PARSING_FILEPARSER_HPP_ */
