/*
 * ParsingStats.cpp
 *
 *  Created on: Jul 6, 2017
 *      Author: galarraga
 */

#include <iostream>

#include "../include/parsing/ParsingStats.hpp"

using namespace std;

namespace fourbench {
namespace parsing {

ParsingStats::ParsingStats(): numberOfTriples(0),
		numberOfSubjects(0), numberOfInputFiles(0),
		numberOfClasses(0), numberOfProperties(0) { }

ParsingStats::~ParsingStats() {}

ostream& operator<<(ostream& strm, const ParsingStats& stats) {
	strm << "Stats {";
	strm << "numberOfTriples: " << stats.numberOfTriples;
	strm << ", numberOfSubjects: " << stats.numberOfSubjects;
	strm << ", numberOfInpuFiles: " << stats.numberOfInputFiles;
	strm << ", numberOfClasses: " << stats.numberOfClasses;
	strm << ", numberOfProperties: " << stats.numberOfProperties;
	strm << "}";
	return strm;
}

} /* namespace provenance */
} /* namespace fourbench */
