/*
 * PopulateStats.hpp
 *
 *  Created on: Aug 22, 2017
 *      Author: galarraga
 */

#ifndef PROVENANCE_POPULATESTATS_HPP_
#define PROVENANCE_POPULATESTATS_HPP_

#include <string>

using namespace std;

namespace fourbench {
namespace provenance {

struct PopulateStats {
	unsigned numberOfProvenanceIdentifiers;
	string familyName;

	PopulateStats();
};

ostream& operator<<(ostream&, const PopulateStats&);

} /* namespace provenance */
} /* namespace fourbench */

#endif /* PROVENANCE_POPULATESTATS_HPP_ */
