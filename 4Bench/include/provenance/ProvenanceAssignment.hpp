/*
 * ProvenanceAssignment.hpp
 *
 *  Created on: Jul 12, 2017
 *      Author: galarraga
 */

#ifndef PROVENANCE_PROVENANCEASSIGNMENT_HPP_
#define PROVENANCE_PROVENANCEASSIGNMENT_HPP_

#include <memory>
#include <unordered_set>

#include "ProvenanceGraph.hpp"

using namespace std;

namespace fourbench {
namespace provenance {

class ProvenanceAssignment {
protected:
	shared_ptr<ProvenanceGraph> graphPtr;
	unordered_set<unsigned> seenIds;
	ProvenanceAssignment(shared_ptr<ProvenanceGraph> graphPtr);
public:
	virtual ~ProvenanceAssignment();
	virtual unsigned nextProvenanceId() = 0;
	shared_ptr<ProvenanceGraph> getGraph() const;
};

} /* namespace provenance */
} /* namespace fourbench */

#endif /* PROVENANCEASSIGNMENT_HPP_ */
