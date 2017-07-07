/*
 * Activity.cpp
 *
 *  Created on: Jul 7, 2017
 *      Author: galarraga
 */

#include <sstream>

#include "../include/provenance/ProvenanceObject.hpp"
#include "../include/provenance/Activity.hpp"

namespace fourbench {
namespace provenance {

Activity::Activity(unsigned id) : ProvenanceObject(id) {
}

Activity::Activity(unsigned id, const string& domain) : ProvenanceObject(id, domain) {
}

Activity::~Activity() {
}

string Activity::getIRI() const {
	stringstream strm;
	strm << domain << "activity/" << id;
	return strm.str();
}

} /* namespace provenance */
} /* namespace fourbench */
