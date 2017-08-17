/*
 * N3FileParser.cpp
 *
 *  Created on: Aug 11, 2017
 *      Author: galarraga
 */

#include <set>
#include <iostream>
#include <vector>
#include <string>
#include <raptor2.h>

#include "../include/conf/Conf.hpp"
#include "../include/utils/string.hpp"
#include "../include/parsing/Triple.hpp"
#include "../include/parsing/N3FileParser.hpp"

using namespace std;
namespace f = fourbench;
namespace fc = fourbench::conf;

namespace fourbench {
namespace parsing {


N3FileParser::N3FileParser(const vector<string>& filenames) : FileParser(filenames) {
	init();
}

Triple* N3FileParser::parse(const string& chunk) {
	vector<string> parts = f::splitOnce(chunk, " <");
	string subject, predicate, object;
	if (parts.size() >= 2) {
		subject = f::trimIRI(parts[0]);
		string tail = parts[1];
		vector<string> tailParts = splitOnce(tail, "> <");
		bool objectIsIRI = true;
		if (tailParts.size() < 2) {
			tailParts = splitOnce(tail, "> \"");
			objectIsIRI = false;
		}

		if (tailParts.size() >= 2) {
			predicate = tailParts[1];
			if (objectIsIRI) {
				object = concat({"<", tailParts[2]});
			} else {
				object = concat({"\"", tailParts[2]});
			}
			auto pos = object.find(" .\n");
			object.replace(pos, 3, "");
			return new Triple(subject, predicate, object, filenames[currentFileIdx], lineNumber);
		} else {
			return nullptr;
		}
	} else {
		return nullptr;
	}
}

bool N3FileParser::getChunk(ifstream& strm, string& output) {
	stringstream sstrm;
	char previous = 0;
	while (!strm.eof()) {
		char c;
		strm.get(c);
		sstrm << c;
		if (c == '\n' && previous == '.') {
			output = f::trim(sstrm.str());
			return true;
		}
		previous = c;
	}

	if (previous == '.') {
		output = f::trim(sstrm.str());
		return true;
	}

	return false;
}

void N3FileParser::init() {
	FileParser::init();
	map<string, set<string>> family2Subjects;
	fc::Conf& conf = fc::Conf::defaultConfig();

	for (auto itr = filenames.begin(); itr != filenames.end(); ++itr) {
		ifstream stream(*itr);
		string line;
		while (getChunk(stream, line)) {
			++numberOfLines;

			// TODO: Implement the right spliting
			vector<string> parts = splitOnce(line, " <");
			if (parts.size() >= 2) {
				string tail = parts[1];
				vector<string> tailParts = splitOnce(tail, "> <");
				if (tailParts.size() < 2) {
					tailParts = splitOnce(tail, "> \"");
				}

				string family = conf.getFamily(tailParts[0]);
				auto itr = numberOfTriplesPerFamily.find(family);
				if (itr == numberOfTriplesPerFamily.end()) {
					numberOfTriplesPerFamily[family] = 1;
				} else{
					itr->second = itr->second + 1;
				}
				family2Subjects[family].insert(f::trim(parts[0]));
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


Triple* N3FileParser::next() {
	// Otherwise fill the buffer
	if (currentFileIdx < 0) {
		currentFileIdx = 0;
	}

	if (currentFileIdx >= filenames.size()) {
		return nullptr;
	}


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
	if (getChunk(*currentStream, line)) {
		return parse(line);
	}

	return nullptr;
}

N3FileParser::~N3FileParser() {
}

} /* namespace provenance */
} /* namespace fourbench */
