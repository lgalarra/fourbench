/*
 * UniformProvenanceAssignment.cpp
 *
 *  Created on: Jul 13, 2017
 *      Author: galarraga
 */

#include <memory>
#include <math.h>
#include <map>
#include <stdlib.h>
#include <iostream>

#include "../include/provenance/ProvenanceGraph.hpp"
#include "../include/provenance/ProvenanceAssignment.hpp"
#include "../include/provenance/UniformProvenanceAssignment.hpp"

using namespace std;

namespace fourbench {
namespace provenance {

UniformProvenanceAssignment::UniformProvenanceAssignment(shared_ptr<ProvenanceGraph> graphPtr) :
		ProvenanceAssignment(graphPtr), numberOfSources(graphPtr->getNumberOfSourceEntities()),
		numberOfLeaves(graphPtr->getNumberOfLeafEntities()), numberOfAssignmentsLatestLeaf(0) {
	unsigned maxsl = max(numberOfLeaves, numberOfSources);
	float numberOfEdges = graphPtr->getSources2LeavesDensity() * (numberOfSources * numberOfLeaves - maxsl) + maxsl;
	sourcesPerLeaf = (unsigned)ceil((float)numberOfEdges / numberOfLeaves);
	cout << "#edges " << numberOfEdges << ", #leaves: " << numberOfLeaves << ", #sources:" << numberOfSources << " #sources-per-leaf: " << sourcesPerLeaf << endl;

	if (graphPtr->isAssignmentPerSubject()) {
		maxItemsPerLeaf = (unsigned)ceil(graphPtr->getNumberOfSubjects()
				/ (float)graphPtr->getNumberOfLeafEntities());
	} else {
		maxItemsPerLeaf = (unsigned)ceil(graphPtr->getNumberOfTriples()
				/ (float)graphPtr->getNumberOfLeafEntities());
	}

}

UniformProvenanceAssignment::~UniformProvenanceAssignment() {}

unsigned UniformProvenanceAssignment::nextProvenanceId() {
	cout << "nextProvenanceId " << graphPtr->getName() << endl;
	if (numberOfAssignmentsLatestLeaf >= maxItemsPerLeaf) {
		++latestLeaf;
		numberOfAssignmentsLatestLeaf = 1;
	} else {
		++numberOfAssignmentsLatestLeaf;
	}

	if (seenIds.count(latestLeaf) == 0) {
		seenIds.insert(latestLeaf);
	} else {
		cout << "returning " << latestLeaf  << endl;
		return latestLeaf;
	}

	ProvenanceAssignment::connectLeafToSources(latestLeaf, sourcesPerLeaf);

	cout << "done, " << latestLeaf << " connected to sources "  << endl;

	return latestLeaf;

}

} /* namespace provenance */
} /* namespace fourbench */
