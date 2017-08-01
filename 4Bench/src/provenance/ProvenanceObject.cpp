/*
 * ProvenanceObject.cpp
 *
 *  Created on: Jul 7, 2017
 *      Author: galarraga
 */

#include <ostream>
#include <iostream>
#include <utility>
#include <map>

#include "../include/utils/string.hpp"
#include "../include/utils/integer.hpp"
#include "../include/datatypes/DataValueBuilder.hpp"
#include "../include/datatypes/DataValue.hpp"
#include "../include/provenance/ProvenanceObject.hpp"
#include "../include/provenance/IRIBuilder.hpp"
#include "../include/provenance/Vocabulary.hpp"


using namespace std;

namespace f = fourbench;
namespace fd = fourbench::datatypes;

namespace fourbench {
namespace provenance {

const map<string, fd::DataType*> ProvenanceObject::attributeTypes = {
		{fd::IntegerType::getInstance().getName(), &fd::IntegerType::getInstance()},
		{fd::BooleanType::getInstance().getName(), &fd::BooleanType::getInstance()},
		{fd::RatioType::getInstance().getName(), &fd::RatioType::getInstance()},
		{fd::DateType::getInstance().getName(), &fd::DateType::getInstance()},
		{fd::DateTimeType::getInstance().getName(), &fd::DateTimeType::getInstance()},
		{fd::StringType::getInstance().getName(), &fd::StringType::getInstance()},
		{fd::IRIType::getInstance().getName(), &fd::IRIType::getInstance()},
		{fd::CountryType::getInstance().getName(), &fd::CountryType::getInstance()},
		{fd::AgentType::getInstance().getName(), &fd::AgentType::getInstance()},
		{fd::ActivityType::getInstance().getName(), &fd::ActivityType::getInstance()},
		{fd::EntityType::getInstance().getName(), &fd::EntityType::getInstance()}
};

static const map<string, fd::DataType*> attributeTypesForRandomSelection = {
		{fd::IntegerType::getInstance().getName(), &fd::IntegerType::getInstance()},
		{fd::BooleanType::getInstance().getName(), &fd::BooleanType::getInstance()},
		{fd::RatioType::getInstance().getName(), &fd::RatioType::getInstance()},
		{fd::DateType::getInstance().getName(), &fd::DateType::getInstance()},
		{fd::DateTimeType::getInstance().getName(), &fd::DateTimeType::getInstance()},
		{fd::StringType::getInstance().getName(), &fd::StringType::getInstance()},
		{fd::IRIType::getInstance().getName(), &fd::IRIType::getInstance()},
		{fd::CountryType::getInstance().getName(), &fd::CountryType::getInstance()}
};

ProvenanceObject::ProvenanceObject(unsigned id) : id(id), domain(IRIBuilder::getDefaultDomain()), maxNumberOfAttributes(0) {
}

ProvenanceObject::ProvenanceObject(unsigned id, const string& domain) :
	id(id), domain(domain), maxNumberOfAttributes(0) {
}

ProvenanceObject::ProvenanceObject(unsigned id, const string& domain, unsigned maxNumberOfAttributes) :
	id(id), domain(domain), maxNumberOfAttributes(maxNumberOfAttributes) {

}


ProvenanceObject::~ProvenanceObject() {
	// TODO Auto-generated destructor stub
}

void ProvenanceObject::initialize() {
	unsigned attributesLeft = 0;
	if (maxNumberOfAttributes > 1) {
		attributesLeft = f::urand(1, maxNumberOfAttributes);
	}

	fd::DataValueBuilder& dtbuilder = fd::DataValueBuilder::getInstance();
	attributes[Vocabulary::id] = dtbuilder.get<fd::IntegerValue>(id);
	if (attributesLeft > 0) {
		for (unsigned i = 0; i < attributesLeft; ++i) {
			map<string, fd::DataType*>::const_iterator it = attributeTypesForRandomSelection.begin();
			advance(it,  i % attributeTypesForRandomSelection.size());
			attributes[f::concat({Vocabulary::property, it->first, "_", to_string(i)})] = it->second->getRandomValue();
		}
	}
}

pair<map<string, shared_ptr<fd::DataValue>>::const_iterator, map<string, shared_ptr<fd::DataValue>>::const_iterator> ProvenanceObject::getAttributeIterators() const {
	return make_pair<map<string, shared_ptr<fd::DataValue>>::const_iterator, map<string, shared_ptr<fd::DataValue>>::const_iterator>(attributes.cbegin(), attributes.cend());
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
