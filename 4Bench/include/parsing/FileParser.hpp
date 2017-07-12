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
#include <vector>
#include <algorithm>


#include "ParsingStats.hpp"
#include "Triple.hpp"

using namespace std;

namespace fourbench {
namespace parsing {

class FileParser {
protected:
	vector<string> filenames;
	int currentFileIdx;
	ifstream* currentStream;
	vector<ifstream*> streams;
	int numberOfLines;
	int lineNumber;
	map<string, unsigned> numberOfTriplesPerFamily;
	map<string, unsigned> numberOfSubjectsPerFamily;

	virtual void init();
	virtual unsigned getNumberOfTriples(const string& family) const;
	virtual unsigned getNumberOfSubjects(const string& family) const;

public:
	virtual Triple* next() = 0;
	virtual ParsingStats getParsingStats(const string& family) const;
	virtual shared_ptr<map<string, ParsingStats>> getAllParsingStats() const;
	int getNumberOfLines() const;
	int getLineNumber() const;
	string getCurrentFile() const;
	FileParser(const vector<string>& filenames);
	virtual ~FileParser();
};

} /* namespace parsing */
} /* namespace fourbench */

#endif /* PARSING_FILEPARSER_HPP_ */
