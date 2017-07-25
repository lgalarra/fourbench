/*
 * ProvenanceObject.cpp
 *
 *  Created on: Jul 7, 2017
 *      Author: galarraga
 */

#include <ostream>

#include "../include/utils/string.hpp"
#include "../include/datatypes/DataValueBuilder.hpp"
#include "../include/datatypes/DataValue.hpp"
#include "../include/provenance/ProvenanceObject.hpp"
#include "../include/provenance/IRIBuilder.hpp"


using namespace std;

namespace f = fourbench;
namespace fd = fourbench::datatypes;

namespace fourbench {
namespace provenance {

ProvenanceObject::ProvenanceObject(unsigned id) : id(id), domain(IRIBuilder::getDefaultDomain()) {
	initialize();
}

ProvenanceObject::ProvenanceObject(unsigned id, const string& domain) :
	id(id), domain(domain) {
	initialize();
}

ProvenanceObject::~ProvenanceObject() {
	// TODO Auto-generated destructor stub
}

void ProvenanceObject::initialize() {
	fd::DataValueBuilder& dtbuilder = fd::DataValueBuilder::getInstance();
	attributes[f::concat({domain, "id"})] = dtbuilder.get<fd::IntegerValue>(id);
}

unsigned ProvenanceObject::getId() const {
	return id;
}

ostream& ProvenanceObject::dump(ostream& o) const {
	o << this->getIRI();
	return o;
}

} /* namespace provenance */
} /* namespace fourbench */
