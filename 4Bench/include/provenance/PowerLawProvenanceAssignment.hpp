/*
 * PowerLawProvenanceAssignment.hpp
 *
 *  Created on: Jul 31, 2017
 *      Author: galarraga
 */

#ifndef PROVENANCE_POWERLAWPROVENANCEASSIGNMENT_HPP_
#define PROVENANCE_POWERLAWPROVENANCEASSIGNMENT_HPP_

#include <memory>

#include "ProvenanceAssignment.hpp"
#include "ProvenanceGraph.hpp"

using namespace std;

namespace fourbench {
namespace provenance {

class PowerLawProvenanceAssignment: public ProvenanceAssignment {
private:
	unsigned powerLaw(unsigned index, float alpha) const;
	unordered_set<unsigned> uniqueids;
public:
	PowerLawProvenanceAssignment(shared_ptr<ProvenanceGraph> graphPtr);
	virtual ~PowerLawProvenanceAssignment();
	unsigned nextProvenanceId();
};

} /* namespace provenance */
} /* namespace fourbench */

#endif /* PROVENANCE_POWERLAWPROVENANCEASSIGNMENT_HPP_ */
