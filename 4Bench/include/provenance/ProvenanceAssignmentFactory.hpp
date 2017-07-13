/*
 * ProvenanceAssignmentBuilder.hpp
 *
 *  Created on: Jul 13, 2017
 *      Author: galarraga
 */

#ifndef PROVENANCE_PROVENANCEASSIGNMENTFACTORY_HPP_
#define PROVENANCE_PROVENANCEASSIGNMENTFACTORY_HPP_

#include <memory>

#include "ProvenanceAssignment.hpp"
#include "ProvenanceGraph.hpp"

using namespace std;

namespace fourbench {
namespace provenance {

class ProvenanceAssignmentFactory {
private:
	static ProvenanceAssignmentFactory instance;
	ProvenanceAssignmentFactory();
public:
	virtual ~ProvenanceAssignmentFactory();
	static ProvenanceAssignmentFactory& getInstance();
	shared_ptr<ProvenanceAssignment> getProvenanceAssignment(shared_ptr<ProvenanceGraph> graphPtr) const;
};

} /* namespace provenance */
} /* namespace fourbench */

#endif /* PROVENANCEASSIGNMENTFACTORY_HPP_ */
