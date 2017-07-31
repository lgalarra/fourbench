/*
 * GeometricProvenanceAssignment.hpp
 *
 *  Created on: Jul 31, 2017
 *      Author: galarraga
 */

#ifndef PROVENANCE_GEOMETRICPROVENANCEASSIGNMENT_HPP_
#define PROVENANCE_GEOMETRICPROVENANCEASSIGNMENT_HPP_

#include "ProvenanceAssignment.hpp"
#include "ProvenanceGraph.hpp"

namespace fourbench {
namespace provenance {

class GeometricProvenanceAssignment: public ProvenanceAssignment {
private:
	unsigned geometric(unsigned index, float alpha) const;
public:
	GeometricProvenanceAssignment(shared_ptr<ProvenanceGraph> graphPtr);
	unsigned nextProvenanceId();
	virtual ~GeometricProvenanceAssignment();
};

} /* namespace provenance */
} /* namespace fourbench */

#endif /* PROVENANCE_GEOMETRICPROVENANCEASSIGNMENT_HPP_ */
