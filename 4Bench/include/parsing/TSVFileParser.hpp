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
protected:
	virtual void init();

public:
	TSVFileParser(const vector<string>& filenames);
	virtual Triple* next();
	virtual ~TSVFileParser();
};

} /* namespace parsing */
} /* namespace fourbench */

#endif /* PARSING_TSVFILEPARSER_HPP_ */
