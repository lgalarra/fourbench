/*
 * ProvenanceAssignment.cpp
 *
 *  Created on: Jul 12, 2017
 *      Author: galarraga
 */

#include <memory>

#include "../include/conf/Conf.hpp"
#include "../include/provenance/ProvenanceAssignment.hpp"

namespace fourbench {
namespace provenance {

namespace fc = fourbench::conf;

ProvenanceAssignment::ProvenanceAssignment(shared_ptr<ProvenanceGraph> graphPtr) : graphPtr(graphPtr) {

}

ProvenanceAssignment::~ProvenanceAssignment() {
	// TODO Auto-generated destructor stub
}

shared_ptr<ProvenanceGraph> ProvenanceAssignment::getGraph() const {
	return graphPtr;
}

} /* namespace provenance */
} /* namespace fourbench */
