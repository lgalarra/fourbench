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
#include <map>


#include "Triple.hpp"

using namespace std;

namespace fourbench {
namespace parsing {

class FileParser {
protected:
	string filename;
	ifstream* stream;
	map<string, unsigned> numberOfLinesPerFamily;
	map<string, unsigned> numberOfSubjectsPerFamily;
	virtual void init();

public:
	virtual Triple* next() = 0;
	virtual unsigned getNumberOfTriples(const string& family) const = 0;
	virtual unsigned getNumberOfSubjects(const string& family) const = 0;
	FileParser(const string& filename);
	virtual ~FileParser();
};

} /* namespace parsing */
} /* namespace fourbench */

#endif /* PARSING_FILEPARSER_HPP_ */
