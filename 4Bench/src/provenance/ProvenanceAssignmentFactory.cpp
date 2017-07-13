/*
 * ProvenanceAssignmentFactory.cpp
 *
 *  Created on: Jul 13, 2017
 *      Author: galarraga
 */

#include <memory>

#include "../include/conf/AssignmentDistribution.hpp"
#include "../include/provenance/ProvenanceGraph.hpp"
#include "../include/provenance/ProvenanceAssignmentFactory.hpp"
#include "../include/provenance/ProvenanceAssignment.hpp"
#include "../include/provenance/UniformProvenanceAssignment.hpp"


using namespace std;
namespace fc = fourbench::conf;

namespace fourbench {
namespace provenance {

ProvenanceAssignmentFactory ProvenanceAssignmentFactory::instance;

ProvenanceAssignmentFactory::ProvenanceAssignmentFactory() {}

ProvenanceAssignmentFactory::~ProvenanceAssignmentFactory() {}

ProvenanceAssignmentFactory& ProvenanceAssignmentFactory::getInstance() {
	return instance;
}

shared_ptr<ProvenanceAssignment> ProvenanceAssignmentFactory::getProvenanceAssignment(
		shared_ptr<ProvenanceGraph> graphPtr) const {
	switch (graphPtr->getEntitiesToTriplesDistribution()) {
	case fc::AssignmentDistribution::UNIFORM:
		return make_shared<UniformProvenanceAssignment>(graphPtr);
	case fc::AssignmentDistribution::GEOMETRIC:
		return make_shared<UniformProvenanceAssignment>(graphPtr);
	case fc::AssignmentDistribution::POWER_LAW:
		return make_shared<UniformProvenanceAssignment>(graphPtr);
	}

	return make_shared<UniformProvenanceAssignment>(graphPtr);
}

} /* namespace provenance */
} /* namespace fourbench */
