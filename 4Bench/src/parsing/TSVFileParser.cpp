/*
 * TSVFileParser.cpp
 *
 *  Created on: Jun 30, 2017
 *      Author: galarraga
 */

#include <vector>

#include "../include/parsing/TSVFileParser.hpp"
#include "../include/parsing/Triple.hpp"
#include "../include/utils/string.hpp"

namespace fc = fourbench::conf;

namespace fourbench {
namespace parsing {

TSVFileParser::TSVFileParser(const string& filename) : FileParser(filename),
		numberOfLines(0), lineNumber(0) {
	init();
}

TSVFileParser::~TSVFileParser() {
}

Triple* TSVFileParser::next() {
	string line;
	if (getline(*stream, line)) {
		vector<string> parts = split(line, "\t");
		if (parts.size() >= 3) {
			++lineNumber;
			return new Triple(parts[0], parts[1], parts[2], lineNumber);
		} else {
			return nullptr;
		}
	} else {
		return nullptr;
	}
}

unsigned TSVFileParser::getNumberOfTriples(const string& family) const {
	return numberOfLines;
}

void TSVFileParser::init() {
	fc::Conf& conf = fc::Conf::defaultConfig();
	string line;
	while (getline(*stream, line)) {
		++numberOfLines;
		vector<string> parts = split(line, "\t");
		if (parts.size() >= 3) {
			string family = conf.getFamily(parts[1]);
			auto itr = numberOfLinesPerFamily.find(family);
			if (itr == numberOfLinesPerFamily.end()) {
				numberOfLinesPerFamily[family] = 0;
			} else{
				numberOfLinesPerFamily[family] = numberOfLinesPerFamily[family] + 1;
			}
		}
	}
	delete stream;
	stream = new ifstream(filename);
}

} /* namespace parsing */
} /* namespace fourbench */
