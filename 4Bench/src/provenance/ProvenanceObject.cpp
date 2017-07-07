/*
 * ProvenanceObject.cpp
 *
 *  Created on: Jul 7, 2017
 *      Author: galarraga
 */

#include <ostream>

#include "../include/provenance/ProvenanceObject.hpp"

namespace fourbench {
namespace provenance {

ProvenanceObject::ProvenanceObject(unsigned id) : id(id),
		domain(ProvenanceObject::defaultDomain) {
}

ProvenanceObject::ProvenanceObject(unsigned id, const string& domain) :
	id(id), domain(domain) {
}

ProvenanceObject::~ProvenanceObject() {
	// TODO Auto-generated destructor stub
}

unsigned ProvenanceObject::getId() const {
	return id;
}

ostream& ProvenanceObject::dump(ostream& o) const {
	return o;
}

} /* namespace provenance */
} /* namespace fourbench */