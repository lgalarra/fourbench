/*
 * Agent.cpp
 *
 *  Created on: Jul 11, 2017
 *      Author: galarraga
 */

#include <string>
#include <iostream>

#include "../include/conf/Conf.hpp"
#include "../include/datatypes/DataType.hpp"
#include "../include/datatypes/DataValueBuilder.hpp"
#include "../include/datatypes/DataValue.hpp"
#include "../include/provenance/IRIBuilder.hpp"
#include "../include/provenance/IRIType.hpp"
#include "../include/provenance/PROVO.hpp"
#include "../include/provenance/FOAF.hpp"
#include "../include/provenance/Vocabulary.hpp"
#include "../include/provenance/RDF.hpp"
#include "../include/provenance/RDFS.hpp"
#include "../include/provenance/Agent.hpp"
#include "../include/provenance/ProvenanceObject.hpp"
#include "../include/utils/string.hpp"


using namespace std;

namespace f = fourbench;
namespace fc = fourbench::conf;
namespace fd = fourbench::datatypes;

namespace fourbench {
namespace provenance {

Agent::Agent(unsigned id) : ProvenanceObject(id, IRIBuilder::getDefaultDomain(), 0) {
	initialize();
}

Agent::Agent(unsigned id, const string& domain) : ProvenanceObject(id, domain, 0) {
	initialize();
}

Agent::Agent(unsigned id, const string& domain, unsigned maxNumberOfProperties) : ProvenanceObject(id, domain, maxNumberOfProperties) {
	initialize();
}

void Agent::initialize() {
	ProvenanceObject::initialize();
	fd::DataValueBuilder& dataBuilder = fd::DataValueBuilder::getInstance();
	attributes[RDF::type] = dataBuilder.get<fd::IRIValue>(PROVO::Agent);
	attributes[RDFS::label] = dataBuilder.get<fd::StringValue>(f::concat({"Agent ", to_string(id)}));
	attributes[Vocabulary::agentType] = fd::AgentType::getInstance().getRandomValue();
	attributes[RDF::type] = dataBuilder.get<fd::IRIValue>(FOAF::Agent);
	fd::AgentTypeEnum type = attributes[Vocabulary::agentType]->getAs<fd::AgentTypeEnum>();
	if (type == fd::AgentTypeEnum::PERSON) {
		attributes[RDF::type] = dataBuilder.get<fd::IRIValue>(FOAF::Person);
	} else if (type == fd::AgentTypeEnum::ORGANIZATION) {
		attributes[RDF::type] = dataBuilder.get<fd::IRIValue>(FOAF::Organization);
	} else {
		attributes[RDF::type] = dataBuilder.get<fd::IRIValue>(Vocabulary::SoftwareAgent);
	}

}

Agent::~Agent() {}

string Agent::getIRI() const {
	shared_ptr<IRIBuilder> builder = IRIBuilder::getInstance(domain);
	return builder->getIRI(IRIType::AGENT, id);
}


} /* namespace provenance */
} /* namespace fourbench */
