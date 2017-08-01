/*
 * GeometricProvenanceAssignment.cpp
 *
 *  Created on: Jul 31, 2017
 *      Author: galarraga
 */

#include <math.h>
#include <iostream>

#include "../include/provenance/ProvenanceAssignment.hpp"
#include "../include/provenance/GeometricProvenanceAssignment.hpp"
#include "../include/provenance/ProvenanceGraph.hpp"

using namespace std;

namespace fourbench {
namespace provenance {

GeometricProvenanceAssignment::GeometricProvenanceAssignment(shared_ptr<ProvenanceGraph> graphPtr) : ProvenanceAssignment(graphPtr) {
	sourcesPerLeaf = (unsigned) max(1.0f, (float)graphPtr->getNumberOfLeafEntities() / graphPtr->getNumberOfSourceEntities());
}

unsigned GeometricProvenanceAssignment::geometric(unsigned index, float alpha) const {
	unsigned N = graphPtr->getNumberOfTriples();
	return (unsigned) ceil((N - 1) * pow(1.0 - alpha, index + 1) + 1);
}

unsigned GeometricProvenanceAssignment::nextProvenanceId() {
	// First calculate the function
	unsigned geo = this->geometric(latestLeaf, graphPtr->getTriples2EntitiesDensity());
	cout << "Leaf with id " << latestLeaf << " should be assigned to " << geo << " triples" << endl;
	unsigned count = seenIds.count(latestLeaf);
	if (count >= geo) {
		++latestLeaf;
	}

	seenIds.insert(latestLeaf);
	ProvenanceAssignment::connectLeafToSources(latestLeaf, sourcesPerLeaf);


	return latestLeaf;
}

GeometricProvenanceAssignment::~GeometricProvenanceAssignment() {
	// TODO Auto-generated destructor stub
}

} /* namespace provenance */
} /* namespace fourbench */
