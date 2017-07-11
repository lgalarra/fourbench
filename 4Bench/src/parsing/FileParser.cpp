/*
 * FileParser.cpp
 *
 *  Created on: Jun 29, 2017
 *      Author: galarraga
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include <string>
#include <map>

#include "../include/parsing/FileParser.hpp"
#include "../include/parsing/ParsingStats.hpp"

using namespace std;

namespace fourbench {
namespace parsing {

FileParser::FileParser(const vector<string>& filenames) : filenames(filenames),
		currentFileIdx(0), currentStream(nullptr) {
	init();
}

FileParser::~FileParser() {
	while (!streams.empty()) {
		vector<ifstream*>::iterator itr = streams.begin();
		ifstream* target = *itr;
		streams.erase(itr);
		delete target;
	}
}

void FileParser::init() {}

shared_ptr<map<string, ParsingStats>> FileParser::getAllParsingStats() const {
	shared_ptr<map<string, ParsingStats>> resultPtr(new map<string, ParsingStats>);
	for (auto itr = numberOfLinesPerFamily.begin(); itr != numberOfLinesPerFamily.end(); ++itr) {
		ParsingStats stat = getParsingStats(itr->first);
		resultPtr.get()->insert(pair<string, ParsingStats>(itr->first, stat));
	}
	return resultPtr;
}

ParsingStats FileParser::getParsingStats(const string& family) const {
	ParsingStats stats;
	stats.numberOfSubjects = getNumberOfSubjects(family);
	stats.numberOfTriples = getNumberOfTriples(family);
	stats.numberOfInputFiles = filenames.size();
	return stats;
}

unsigned FileParser::getNumberOfTriples(const string& family) const {
	auto itr = numberOfLinesPerFamily.find(family);
	if (itr == numberOfLinesPerFamily.end()) {
		return -1;
	} else {
		return itr->second;
	}
}

unsigned FileParser::getNumberOfSubjects(const string& family) const {
	auto itr = numberOfSubjectsPerFamily.find(family);
	if (itr == numberOfSubjectsPerFamily.end()) {
		return -1;
	} else {
		return itr->second;
	}
}

} /* namespace parsing */
} /* namespace fourbench */
