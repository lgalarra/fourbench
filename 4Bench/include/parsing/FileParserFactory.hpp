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
#include <memory>

using namespace std;

namespace fourbench {
namespace parsing {


class FileParserFactory {

private:
	static FileParserFactory instance;

public:
	FileParserFactory();
	static FileParserFactory& getInstance();

	template <class Parser> shared_ptr<Parser> buildParser(const vector<string>& filenames) {
		return make_shared<Parser>(filenames);
	}


	virtual ~FileParserFactory();
};

} /* namespace parsing */
} /* namespace fourbench */

#endif /* PARSING_FILEPARSERFACTORY_HPP_ */
