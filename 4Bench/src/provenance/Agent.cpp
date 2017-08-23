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
	types.insert(dataBuilder.get<fd::IRIValue>(PROVO::Agent));
	types.insert(dataBuilder.get<fd::IRIValue>(FOAF::Agent));
	attributes[RDFS::label] = dataBuilder.get<fd::StringValue>(f::concat({"Agent ", to_string(id)}));
	fd::AgentTypeEnum type = fd::AgentType::getInstance().getRandomValue()->getAs<fd::AgentTypeEnum>();
	if (type == fd::AgentTypeEnum::PERSON) {
		types.insert(dataBuilder.get<fd::IRIValue>(PROVO::Person));
	} else if (type == fd::AgentTypeEnum::ORGANIZATION) {
		types.insert(dataBuilder.get<fd::IRIValue>(PROVO::Organization));
	} else {
		types.insert(dataBuilder.get<fd::IRIValue>(PROVO::SoftwareAgent));
	}
}

Agent::~Agent() {}

string Agent::getIRI() const {
	shared_ptr<IRIBuilder> builder = IRIBuilder::getInstance(domain);
	return builder->getIRI(IRIType::AGENT, id);
}


} /* namespace provenance */
} /* namespace fourbench */
