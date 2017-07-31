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
		numberOfLeaves(graphPtr->getNumberOfLeafEntities()), allSourcesConnected(false),
		latestLeaf(graphPtr->getFirstLeafId()), numberOfAssignmentsLatestLeaf(0) {
	unsigned maxsl = max(numberOfLeaves, numberOfSources);
	float numberOfEdges = graphPtr->getSources2LeavesDensity() * (numberOfSources * numberOfLeaves - maxsl) + maxsl;
	sourcesPerLeaf = (unsigned)max(1.0f, ceil(numberOfEdges / numberOfLeaves));
	// All sources have no leaves connected to them
	for (unsigned i = 0; i < numberOfSources; ++i) {
		sourcesPriorityQueue.push(pair<unsigned, unsigned>(i, 0));
	}

	if (graphPtr->isAssignmentPerSubject()) {
		maxItemsPerLeaf = (unsigned)ceil(graphPtr->getNumberOfSubjects()
				/ (float)graphPtr->getNumberOfLeafEntities());
	} else {
		maxItemsPerLeaf = (unsigned)ceil(graphPtr->getNumberOfTriples()
				/ (float)graphPtr->getNumberOfLeafEntities());
	}

}

UniformProvenanceAssignment::~UniformProvenanceAssignment() {
	// TODO Auto-generated destructor stub
}

unsigned UniformProvenanceAssignment::nextProvenanceId() {
	cout << "nextProvenanceId" << endl;
	if (numberOfAssignmentsLatestLeaf >= maxItemsPerLeaf) {
		++latestLeaf;
		numberOfAssignmentsLatestLeaf = 1;
	} else {
		++numberOfAssignmentsLatestLeaf;
	}

	if (seenIds.find(latestLeaf) == seenIds.end()) {
		seenIds.insert(latestLeaf);
	} else {
		cout << "returning " << latestLeaf  << endl;
		return latestLeaf;
	}

	if (graphPtr->getDepth() > 1) {
		if (!allSourcesConnected) {
			cout << "Sources per leaf " << sourcesPerLeaf << endl;
			// Pick the top N unconnected sources
			for (unsigned i = 0; i < sourcesPerLeaf; ++i) {
				pair<unsigned, unsigned> top = sourcesPriorityQueue.top();
				cout << "Top " << top.first << ", " << top.second << " ";
				if (top.second > 0) {
					// This means all sources had been used once
					allSourcesConnected = true;
				}
				top.second = top.second + 1;
				cout << "Using priority queue. ";
				graphPtr->connectSourceAndLeaf(graphPtr->getSourceAbsoluteId(top.first), latestLeaf);
				sourcesPriorityQueue.pop();
				sourcesPriorityQueue.push(top);
			}
		} else {
			// Pick random sources
			for (unsigned i = 0; i < sourcesPerLeaf; ++i) {
				unsigned sourceIdx = rand() % numberOfSources;
				cout << "Using random assignment. ";
				graphPtr->connectSourceAndLeaf(graphPtr->getSourceAbsoluteId(sourceIdx), latestLeaf);
			}
		}
	}

	cout << "done, " << latestLeaf << " connected to sources "  << endl;

	return latestLeaf;

}

} /* namespace provenance */
} /* namespace fourbench */
