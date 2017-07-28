/*
 * Entity.cpp
 *
 *  Created on: Jul 11, 2017
 *      Author: galarraga
 */

#include "../include/conf/Conf.hpp"
#include "../include/datatypes/DataValueBuilder.hpp"
#include "../include/datatypes/DataValue.hpp"
#include "../include/provenance/EntityLevel.hpp"
#include "../include/provenance/ProvenanceObject.hpp"
#include "../include/provenance/Entity.hpp"
#include "../include/provenance/IRIBuilder.hpp"
#include "../include/provenance/IRIType.hpp"
#include "../include/provenance/Vocabulary.hpp"
#include "../include/provenance/PROVO.hpp"
#include "../include/provenance/RDF.hpp"
#include "../include/provenance/RDFS.hpp"
#include "../include/utils/string.hpp"



namespace fd = fourbench::datatypes;
namespace f = fourbench;

namespace fourbench {
namespace provenance {

Entity::Entity(unsigned id) : ProvenanceObject(id, IRIBuilder::getDefaultDomain(), 0), level(EntityLevel::UNDEFINED) {

}

Entity::Entity(unsigned id, const string& domain): ProvenanceObject(id, domain, 0), level(EntityLevel::UNDEFINED) {

}

Entity::Entity(unsigned id, const string& domain, unsigned maxNumberOfAttributes) :
	ProvenanceObject(id, domain, maxNumberOfAttributes), level(EntityLevel::UNDEFINED) {

}

Entity::Entity(unsigned id, const string& domain, unsigned maxNumberOfProperties, EntityLevel level) :
	ProvenanceObject(id, domain, maxNumberOfProperties), level(level) {

}

void Entity::initialize() {
	fd::DataValueBuilder& dataBuilder = fd::DataValueBuilder::getInstance();
	if (level == EntityLevel::SOURCE_AND_LEAF || level == EntityLevel::SOURCE
			|| level == EntityLevel::LEAF) {
		ProvenanceObject::initialize();
	} else {
		attributes[f::concat({domain, "id"})] = dataBuilder.get<fd::IntegerValue>(id);
	}

	attributes[RDF::type] = dataBuilder.get<fd::IRIValue>(PROVO::Entity);
	attributes[RDFS::label] = dataBuilder.get<fd::StringValue>(f::concat({"Entity ", to_string(id)}));

	if (level == EntityLevel::SOURCE_AND_LEAF || level == EntityLevel::SOURCE) {
		attributes[Vocabulary::sourceType] = fd::EntityType::getInstance().getRandomValue();
	}
}


Entity::~Entity() {
}

string Entity::getIRI() const {
	shared_ptr<IRIBuilder> builderPtr = IRIBuilder::getInstance(domain);
	return builderPtr->getIRI(IRIType::ENTITY, id);
}

} /* namespace provenance */
} /* namespace fourbench */
