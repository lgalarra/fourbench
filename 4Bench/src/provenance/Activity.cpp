/*
 * Activity.cpp
 *
 *  Created on: Jul 7, 2017
 *      Author: galarraga
 */

#include <sstream>

#include "../include/provenance/ProvenanceObject.hpp"
#include "../include/provenance/Activity.hpp"
#include "../include/provenance/IRIBuilder.hpp"
#include "../include/provenance/IRIType.hpp"

namespace fourbench {
namespace provenance {

Activity::Activity(unsigned id) : ProvenanceObject(id) {
}

Activity::Activity(unsigned id, const string& domain) : ProvenanceObject(id, domain) {
}

Activity::~Activity() {
}

string Activity::getIRI() const {
	return IRIBuilder::getInstance().getIRI(domain, IRIType::ACTIVITY, id);
}

} /* namespace provenance */
} /* namespace fourbench */
