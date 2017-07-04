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

public:
	TSVFileParser(const string& filename);
	virtual Triple* next();
	virtual unsigned getNumberOfTriples(const string& family) const;
	virtual unsigned getNumberOfSubjects(const string& family) const;

	virtual ~TSVFileParser();
};

} /* namespace parsing */
} /* namespace fourbench */

#endif /* PARSING_TSVFILEPARSER_HPP_ */
