/*
 * PopulateStats.cpp
 *
 *  Created on: Aug 22, 2017
 *      Author: galarraga
 */

#include <iostream>

#include "../include/provenance/PopulateStats.hpp"

using namespace std;

namespace fourbench {
namespace provenance {

PopulateStats::PopulateStats() : numberOfProvenanceIdentifiers(0),
		familyName(""){
}

ostream& operator<<(ostream& strm, const PopulateStats& stats) {
	strm << "Output Stats { familyName: " << stats.familyName << ", #prov-identifiers: " << stats.numberOfProvenanceIdentifiers << "}";
	return strm;
}

} /* namespace provenance */
} /* namespace fourbench */
