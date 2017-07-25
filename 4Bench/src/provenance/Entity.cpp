/*
 * Entity.cpp
 *
 *  Created on: Jul 11, 2017
 *      Author: galarraga
 */

#include "../include/datatypes/DataValueBuilder.hpp"
#include "../include/datatypes/DataValue.hpp"
#include "../include/provenance/ProvenanceObject.hpp"
#include "../include/provenance/Entity.hpp"
#include "../include/provenance/IRIBuilder.hpp"
#include "../include/provenance/IRIType.hpp"
#include "../include/provenance/PROVO.hpp"
#include "../include/provenance/RDF.hpp"
#include "../include/provenance/RDFS.hpp"
#include "../include/utils/string.hpp"



namespace fd = fourbench::datatypes;
namespace f = fourbench;

namespace fourbench {
namespace provenance {

Entity::Entity(unsigned id) : ProvenanceObject(id) {

}

Entity::Entity(unsigned id, const string& domain): ProvenanceObject(id, domain) {

}

void Entity::initialize() {
	ProvenanceObject::initialize();
	fd::DataValueBuilder& dataBuilder = fd::DataValueBuilder::getInstance();
	attributes[RDF::type] = dataBuilder.get<fd::IRIValue>(PROVO::Entity);
	attributes[RDFS::label] = dataBuilder.get<fd::StringValue>(f::concat({"Entity ", to_string(id)}));

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
