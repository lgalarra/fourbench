/*
 * FOAF.hpp
 *
 *  Created on: Jul 28, 2017
 *      Author: galarraga
 */

#ifndef PROVENANCE_FOAF_HPP_
#define PROVENANCE_FOAF_HPP_

#include <string>

namespace fourbench {
namespace provenance {

using namespace std;

class FOAF {
private:
	FOAF();
public:
	static const string prefix;
	static const string Agent;
	static const string Person;
	static const string Organization;

	virtual ~FOAF();
};

} /* namespace provenance */
} /* namespace fourbench */

#endif /* PROVENANCE_FOAF_HPP_ */
