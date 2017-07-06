/*
 * ParsingStats.cpp
 *
 *  Created on: Jul 6, 2017
 *      Author: galarraga
 */

#include "../include/parsing/ParsingStats.hpp"

namespace fourbench {
namespace parsing {

ParsingStats::ParsingStats(): numberOfTriples(0),
		numberOfSubjects(0), numberOfInputFiles(0),
		numberOfClasses(0), numberOfProperties(0) { }

ParsingStats::~ParsingStats() {}

} /* namespace provenance */
} /* namespace fourbench */
