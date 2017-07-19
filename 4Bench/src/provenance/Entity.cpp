/*
 * Entity.cpp
 *
 *  Created on: Jul 11, 2017
 *      Author: galarraga
 */

#include "../include/provenance/ProvenanceObject.hpp"
#include "../include/provenance/Entity.hpp"
#include "../include/provenance/IRIBuilder.hpp"
#include "../include/provenance/IRIType.hpp"


namespace fourbench {
namespace provenance {

Entity::Entity(unsigned id) : ProvenanceObject(id) {

}

Entity::Entity(unsigned id, const string& domain): ProvenanceObject(id, domain) {

}


Entity::~Entity() {
	// TODO Auto-generated destructor stub
}

string Entity::getIRI() const {
	shared_ptr<IRIBuilder> builderPtr = IRIBuilder::getInstance(domain);
	return builderPtr->getIRI(IRIType::ENTITY, id);
}



} /* namespace provenance */
} /* namespace fourbench */
