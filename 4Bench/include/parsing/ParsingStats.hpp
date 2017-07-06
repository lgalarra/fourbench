/*
 * ParsingStats.hpp
 *
 *  Created on: Jul 6, 2017
 *      Author: galarraga
 */

#ifndef PARSING_PARSINGSTATS_HPP_
#define PARSING_PARSINGSTATS_HPP_

namespace fourbench {
namespace parsing {

class ParsingStats {
public:
	unsigned numberOfSubjects;
	unsigned numberOfTriples;
	unsigned numberOfInputFiles;
	unsigned numberOfClasses;
	unsigned numberOfProperties;

	ParsingStats();
	virtual ~ParsingStats();
};

} /* namespace provenance */
} /* namespace fourbench */

#endif /* PARSING_PARSINGSTATS_HPP_ */
