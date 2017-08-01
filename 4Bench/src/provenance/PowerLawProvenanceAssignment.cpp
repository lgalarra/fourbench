/*
 * PowerLawProvenanceAssignment.cpp
 *
 *  Created on: Jul 31, 2017
 *      Author: galarraga
 */

#include <memory>
#include <math.h>
#include <iostream>

#include "../include/provenance/PowerLawProvenanceAssignment.hpp"
#include "../include/provenance/ProvenanceGraph.hpp"

using namespace std;

namespace fourbench {
namespace provenance {

PowerLawProvenanceAssignment::PowerLawProvenanceAssignment(shared_ptr<ProvenanceGraph> graphPtr) : ProvenanceAssignment(graphPtr) {
	sourcesPerLeaf = (unsigned) max(1.0f, (float)graphPtr->getNumberOfLeafEntities() / graphPtr->getNumberOfSourceEntities());
}

PowerLawProvenanceAssignment::~PowerLawProvenanceAssignment() {
	// TODO Auto-generated destructor stub
}

unsigned PowerLawProvenanceAssignment::powerLaw(unsigned index, float alpha) const {
	unsigned N = graphPtr->getNumberOfTriples();
	float denominator = pow(N - index, alpha);
	if (denominator <= 0) {
		cerr << "PowerLawProvenanceAssignment::powerLaw, division by non-positive real number: " << denominator << endl;
		denominator = N;
	}
	return (unsigned) max(1.0f, floor(N / denominator));
}

unsigned PowerLawProvenanceAssignment::nextProvenanceId() {
	// First calculate the function
	unsigned powerlaw = powerLaw(latestLeaf, graphPtr->getTriples2EntitiesDensity());
	cout << "Leaf with id " << latestLeaf << " should be assigned to " << powerlaw << " triples" << endl;
	unsigned count = seenIds.count(latestLeaf);
	if (count >= powerlaw) {
		++latestLeaf;
	}

	seenIds.insert(latestLeaf);
	cout << "Calling ProvenanceAssignment::connectLeafToSources(" << latestLeaf << ", " << sourcesPerLeaf << ")" << endl;
	ProvenanceAssignment::connectLeafToSources(latestLeaf, sourcesPerLeaf);


	return latestLeaf;


}

} /* namespace provenance */
} /* namespace fourbench */
