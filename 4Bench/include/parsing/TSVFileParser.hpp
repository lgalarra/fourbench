/*
 * TSVFileParser.hpp
 *
 *  Created on: Jun 30, 2017
 *      Author: galarraga
 */

#ifndef PARSING_TSVFILEPARSER_HPP_
#define PARSING_TSVFILEPARSER_HPP_

#include <map>
#include <string>
#include <vector>

#include "../include/conf/Conf.hpp"
#include "FileParser.hpp"
#include "Triple.hpp"

namespace fc = fourbench::conf;
using namespace std;

namespace fourbench {
namespace parsing {

class TSVFileParser : public FileParser {
private:
	unsigned numberOfLines;
	unsigned lineNumber;


protected:
	virtual void init();
	virtual unsigned getNumberOfTriples(const string& family) const;
	virtual unsigned getNumberOfSubjects(const string& family) const;

public:
	TSVFileParser(const vector<string>& filenames);
	virtual Triple* next();
	virtual ParsingStats getParsingStats(const string& family) const;
	virtual ~TSVFileParser();
};

} /* namespace parsing */
} /* namespace fourbench */

#endif /* PARSING_TSVFILEPARSER_HPP_ */
