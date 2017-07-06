/*
 * FileParserFactory.hpp
 *
 *  Created on: Jun 30, 2017
 *      Author: galarraga
 */

#ifndef PARSING_FILEPARSERFACTORY_HPP_
#define PARSING_FILEPARSERFACTORY_HPP_

#include <string>
#include <vector>

using namespace std;

namespace fourbench {
namespace parsing {


class FileParserFactory {

private:
	static FileParserFactory instance;

public:
	FileParserFactory();
	static FileParserFactory& getInstance();

	template <class Parser> Parser* buildParser(const vector<string>& filenames) {
		Parser* parser = new Parser(filenames);
		return parser;
	}


	virtual ~FileParserFactory();
};

} /* namespace parsing */
} /* namespace fourbench */

#endif /* PARSING_FILEPARSERFACTORY_HPP_ */
