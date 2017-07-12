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
		currentFileIdx(-1), currentStream(nullptr), numberOfLines(0), lineNumber(0) {
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

void FileParser::init() {
	numberOfTriplesPerFamily["default"] = 0;
	numberOfSubjectsPerFamily["default"] = 0;
}

int FileParser::getNumberOfLines() const {
	return numberOfLines;
}

int FileParser::getLineNumber() const {
	return lineNumber;
}

string FileParser::getCurrentFile() const {
	if (currentFileIdx >= 0 && currentFileIdx < filenames.size()) {
		return filenames[currentFileIdx];
	} else {
		return currentFileIdx < 0 ? "Parsing not started" : "Parsing finalized";
	}
}

shared_ptr<map<string, ParsingStats>> FileParser::getAllParsingStats() const {
	shared_ptr<map<string, ParsingStats>> resultPtr(new map<string, ParsingStats>);
	for (auto itr = numberOfTriplesPerFamily.begin(); itr != numberOfTriplesPerFamily.end(); ++itr) {
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
	auto itr = numberOfTriplesPerFamily.find(family);
	if (itr == numberOfTriplesPerFamily.end()) {
		return 0;
	} else {
		return itr->second;
	}
}

unsigned FileParser::getNumberOfSubjects(const string& family) const {
	auto itr = numberOfSubjectsPerFamily.find(family);
	if (itr == numberOfSubjectsPerFamily.end()) {
		return 0;
	} else {
		return itr->second;
	}
}

} /* namespace parsing */
} /* namespace fourbench */
