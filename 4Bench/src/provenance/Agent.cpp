/*
 * Agent.cpp
 *
 *  Created on: Jul 11, 2017
 *      Author: galarraga
 */

#include <string>

#include "../include/provenance/IRIBuilder.hpp"
#include "../include/provenance/IRIType.hpp"
#include "../include/provenance/Agent.hpp"
#include "../include/provenance/ProvenanceObject.hpp"

using namespace std;

namespace fourbench {
namespace provenance {

Agent::Agent(unsigned id) : ProvenanceObject(id) {}

Agent::Agent(unsigned id, const string& domain) : ProvenanceObject(id, domain) {}

Agent::~Agent() {
	// TODO Auto-generated destructor stub
}

string Agent::getIRI() const {
	shared_ptr<IRIBuilder> builder = IRIBuilder::getInstance(domain);
	return builder->getIRI(IRIType::AGENT, id);
}

} /* namespace provenance */
} /* namespace fourbench */
