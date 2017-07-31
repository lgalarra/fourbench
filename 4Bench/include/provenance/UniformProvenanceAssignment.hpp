/*
 * UniformProvenanceAssignment.hpp
 *
 *  Created on: Jul 13, 2017
 *      Author: galarraga
 */

#ifndef PROVENANCE_UNIFORMPROVENANCEASSIGNMENT_HPP_
#define PROVENANCE_UNIFORMPROVENANCEASSIGNMENT_HPP_

#include <memory>
#include <map>

#include "ProvenanceAssignment.hpp"
#include "ProvenanceGraph.hpp"

using namespace std;

namespace fourbench {
namespace provenance {

class UniformProvenanceAssignment : public ProvenanceAssignment {
private:
	unsigned numberOfSources;
	unsigned numberOfLeaves;
	unsigned numberOfAssignmentsLatestLeaf;
	unsigned maxItemsPerLeaf;

public:
	UniformProvenanceAssignment(shared_ptr<ProvenanceGraph> graphPtr);
	virtual ~UniformProvenanceAssignment();
	virtual unsigned nextProvenanceId();
};

} /* namespace provenance */
} /* namespace fourbench */

#endif /* PROVENANCE_UNIFORMPROVENANCEASSIGNMENT_HPP_ */
