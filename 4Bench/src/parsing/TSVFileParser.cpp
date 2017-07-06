/*
 * TSVFileParser.cpp
 *
 *  Created on: Jun 30, 2017
 *      Author: galarraga
 */

#include <vector>

#include "../include/conf/Conf.hpp"
#include "../include/parsing/TSVFileParser.hpp"
#include "../include/parsing/ParsingStats.hpp"
#include "../include/parsing/Triple.hpp"
#include "../include/utils/string.hpp"

namespace fc = fourbench::conf;

namespace fourbench {
namespace parsing {

TSVFileParser::TSVFileParser(const vector<string>& filenames) : FileParser(filenames),
		numberOfLines(0), lineNumber(0) {
	init();
}

TSVFileParser::~TSVFileParser() {
}

Triple* TSVFileParser::next() {
	if (currentStream == nullptr) {
		return nullptr;
	}

	if (currentStream->eof()) {
		currentFileIdx++;
		// We have scanned all files
		if (currentFileIdx >= filenames.size()) {
			currentStream = nullptr;
			return nullptr;
		} else {
			currentStream = streams[currentFileIdx];
		}
	}

	string line;
	if (getline(*currentStream, line)) {
		vector<string> parts = split(line, "\t");
		if (parts.size() >= 3) {
			++lineNumber;
			return new Triple(parts[0], parts[1],
					parts[2], filenames[currentFileIdx], lineNumber);
		} else {
			return nullptr;
		}
	} else {
		return nullptr;
	}
}

ParsingStats TSVFileParser::getParsingStats(const string& family) const {
	ParsingStats stats;
	stats.numberOfSubjects = getNumberOfSubjects(family);
	stats.numberOfTriples = getNumberOfTriples(family);
	stats.numberOfInputFiles = filenames.size();
	return stats;
}

unsigned TSVFileParser::getNumberOfTriples(const string& family) const {
	auto itr = numberOfLinesPerFamily.find(family);
	if (itr == numberOfLinesPerFamily.end()) {
		return -1;
	} else {
		return itr->second;
	}
}

unsigned TSVFileParser::getNumberOfSubjects(const string& family) const {
	auto itr = numberOfSubjectsPerFamily.find(family);
	if (itr == numberOfSubjectsPerFamily.end()) {
		return -1;
	} else {
		return itr->second;
	}
}

void TSVFileParser::init() {
	map<string, set<string>> family2Subjects;
	fc::Conf& conf = fc::Conf::defaultConfig();

	for (auto itr = filenames.begin(); itr != filenames.end(); ++itr) {
		ifstream stream(*itr);
		string line;
		while (getline(stream, line)) {
			++numberOfLines;
			vector<string> parts = split(line, "\t");
			if (parts.size() >= 3) {
				string family = conf.getFamily(parts[1]);
				auto itr = numberOfLinesPerFamily.find(family);
				if (itr == numberOfLinesPerFamily.end()) {
					numberOfLinesPerFamily[family] = 1;
				} else{
					itr->second = itr->second + 1;
				}
				family2Subjects[family].insert(parts[0]);
			}
		}

		for (auto itr = family2Subjects.begin(); itr != family2Subjects.end(); ++itr) {
			numberOfSubjectsPerFamily[itr->first] = itr->second.size();
		}
	}

	for (auto itr = filenames.begin(); itr != filenames.end(); ++itr) {
		// Prepare the streams for future access
		streams.push_back(new ifstream(*itr));
	}

	if (!streams.empty()) {
		currentStream = *streams.begin();
	}
}

} /* namespace parsing */
} /* namespace fourbench */
