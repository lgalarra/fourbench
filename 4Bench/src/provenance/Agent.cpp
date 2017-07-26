/*
 * Agent.cpp
 *
 *  Created on: Jul 11, 2017
 *      Author: galarraga
 */

#include <string>

#include "../include/conf/Conf.hpp"
#include "../include/datatypes/DataValueBuilder.hpp"
#include "../include/datatypes/DataValue.hpp"
#include "../include/provenance/IRIBuilder.hpp"
#include "../include/provenance/IRIType.hpp"
#include "../include/provenance/PROVO.hpp"
#include "../include/provenance/RDF.hpp"
#include "../include/provenance/RDFS.hpp"
#include "../include/provenance/Agent.hpp"
#include "../include/provenance/ProvenanceObject.hpp"
#include "../include/utils/string.hpp"


using namespace std;

namespace f = fourbench;
namespace fc = fourbench::conf;

namespace fourbench {
namespace provenance {

Agent::Agent(unsigned id) : ProvenanceObject(id) {}

Agent::Agent(unsigned id, const string& domain) : ProvenanceObject(id, domain) {}

void Agent::initialize() {
	ProvenanceObject::initialize();
	fd::DataValueBuilder& dataBuilder = fd::DataValueBuilder::getInstance();
	attributes[RDF::type] = dataBuilder.get<fd::IRIValue>(PROVO::Agent);
	attributes[RDFS::label] = dataBuilder.get<fd::StringValue>(f::concat({"Agent ", to_string(id)}));

}

Agent::~Agent() {}

string Agent::getIRI() const {
	shared_ptr<IRIBuilder> builder = IRIBuilder::getInstance(domain);
	return builder->getIRI(IRIType::AGENT, id);
}

void Agent::populateWithAttributes(const fc::ConfValues& conf) {

}

} /* namespace provenance */
} /* namespace fourbench */
