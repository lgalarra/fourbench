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
		subject = parts[0];
		string tail = parts[1];
		vector<string> tailParts = splitOnce(tail, "> ");

		if (tailParts.size() < 2) {
			return nullptr;
		}

		predicate = concat({"<", tailParts[0], ">"});
		object = tailParts[1];

		auto pos = object.find(" .\n");
		auto posQuad = object.find(" <");
		if (posQuad != string::npos) {
			pos = posQuad;
		} else {
			if (pos == string::npos) {
				cerr << subject << ", " <<  predicate << ", " << object <<  ": Problem at parsing line " << lineNumber << endl;
				return nullptr;
			}
		}
		object = f::trim(string(object.begin(), object.begin() + pos));
		if (currentFileIdx >= 0) {
			return new Triple(subject, predicate, object, filenames[currentFileIdx], lineNumber);
		} else {
			return new Triple(subject, predicate, object);
		}

	} else {
		return nullptr;
	}
}

bool N3FileParser::getChunk(ifstream& strm, string& output) {
	stringstream outStrm;
	char previous = 0;
	bool commentState = false;
	while (!strm.eof()) {
		char c;
		strm.get(c);

		if (c == '\n') {
			++lineNumber;
		}

		if (!commentState && c == '\n' && previous == '.') {
			output = f::trim(outStrm.str());
			return true;
		} else if (!commentState && std::isspace(c) && previous == '#') {
			outStrm.seekp(-1, ios_base::end);
			commentState = true;
		} else if (commentState && c == '\n') {
			commentState = false;
			previous = 0;
		}

		if (!commentState) {
			outStrm << c;
		}

		previous = c;
	}

	if (!commentState && previous == '.') {
		output = f::trim(outStrm.str());
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

			Triple* t = parse(line);
			if (t != nullptr) {
				string family = conf.getFamily(t->getPredicate());
				auto itr = numberOfTriplesPerFamily.find(family);
				if (itr == numberOfTriplesPerFamily.end()) {
					numberOfTriplesPerFamily[family] = 1;
				} else{
					itr->second = itr->second + 1;
				}
				family2Subjects[family].insert(t->getSubject());
				delete t;
			} else {
				cerr << "[Initializing] Problem at parsing: " << line << endl;
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
			lineNumber = 0; // Reset line number
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
