/*
 * ProvenanceAssignment.cpp
 *
 *  Created on: Jul 12, 2017
 *      Author: galarraga
 */

#include "../include/provenance/ProvenanceAssignment.hpp"

namespace fourbench {
namespace provenance {

ProvenanceAssignment::ProvenanceAssignment(shared_ptr<ProvenanceGraph> graphPtr) : graphPtr(graphPtr) {

}

ProvenanceAssignment::~ProvenanceAssignment() {
	// TODO Auto-generated destructor stub
}

} /* namespace provenance */
} /* namespace fourbench */
