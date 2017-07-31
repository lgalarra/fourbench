/*
 * ProvenanceAssignment.cpp
 *
 *  Created on: Jul 12, 2017
 *      Author: galarraga
 */

#include <memory>
#include <iostream>


#include "../include/conf/Conf.hpp"
#include "../include/provenance/ProvenanceAssignment.hpp"

using namespace std;

namespace fourbench {
namespace provenance {

namespace fc = fourbench::conf;

ProvenanceAssignment::ProvenanceAssignment(shared_ptr<ProvenanceGraph> graphPtr) :
		graphPtr(graphPtr), allSourcesConnected(false),
		latestLeaf(graphPtr->getFirstLeafId()), sourcesPerLeaf(1) {
}

ProvenanceAssignment::~ProvenanceAssignment() {
	// TODO Auto-generated destructor stub
}

shared_ptr<ProvenanceGraph> ProvenanceAssignment::getGraph() const {
	return graphPtr;
}

void ProvenanceAssignment::connectLeafToSources(unsigned leafId, unsigned sourcesPerLeaf) {
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
				graphPtr->connectSourceAndLeaf(graphPtr->getSourceAbsoluteId(top.first), leafId);
				sourcesPriorityQueue.pop();
				sourcesPriorityQueue.push(top);
			}
		} else {
			// Pick random sources
			for (unsigned i = 0; i < sourcesPerLeaf; ++i) {
				unsigned sourceIdx = rand() % graphPtr->getNumberOfSourceEntities();
				cout << "Using random assignment. ";
				graphPtr->connectSourceAndLeaf(graphPtr->getSourceAbsoluteId(sourceIdx), leafId);
			}
		}
	}
}

} /* namespace provenance */
} /* namespace fourbench */
