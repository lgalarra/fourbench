/*
 * ProvenanceAssignment.hpp
 *
 *  Created on: Jul 12, 2017
 *      Author: galarraga
 */

#ifndef PROVENANCE_PROVENANCEASSIGNMENT_HPP_
#define PROVENANCE_PROVENANCEASSIGNMENT_HPP_

#include <memory>

using namespace std;

namespace fourbench {
namespace provenance {

class ProvenanceAssignment {
private:
	shared_ptr<ProvenanceGraph> graphPtr;
public:
	ProvenanceAssignment(shared_ptr<ProvenanceGraph> graphPtr);
	virtual ~ProvenanceAssignment();
};

} /* namespace provenance */
} /* namespace fourbench */

#endif /* PROVENANCEASSIGNMENT_HPP_ */
