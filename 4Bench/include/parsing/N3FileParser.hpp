/*
 * N3FileParser.hpp
 *
 *  Created on: Aug 11, 2017
 *      Author: galarraga
 */

#ifndef PARSING_N3FILEPARSER_HPP_
#define PARSING_N3FILEPARSER_HPP_

#include <vector>
#include <string>

#include "FileParser.hpp"
#include "Triple.hpp"

using namespace std;

namespace fourbench {
namespace parsing {

class N3FileParser: public FileParser {
private:
	Triple* parse(const string& chunk);
	bool getChunk(ifstream& strm, string& output);
protected:
	void init();
public:
	N3FileParser(const vector<string>& filenames);
	Triple* next();
	virtual ~N3FileParser();
};

} /* namespace provenance */
} /* namespace fourbench */

#endif /* PARSING_N3FILEPARSER_HPP_ */
