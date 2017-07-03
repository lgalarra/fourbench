/*
 * TSVFileParser.hpp
 *
 *  Created on: Jun 30, 2017
 *      Author: galarraga
 */

#ifndef PARSING_TSVFILEPARSER_HPP_
#define PARSING_TSVFILEPARSER_HPP_

#include <string>

#include "../include/parsing/FileParser.hpp"
#include "../include/parsing/Triple.hpp"

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
	virtual unsigned getNumberOfTriples() const;

	virtual ~TSVFileParser();
};

} /* namespace parsing */
} /* namespace fourbench */

#endif /* PARSING_TSVFILEPARSER_HPP_ */
