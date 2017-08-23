/*
 * ProvenanceAssignment.cpp
 *
 *  Created on: Jul 12, 2017
 *      Author: galarraga
 */

#include <memory>
#include <algorithm>
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
	// All sources have no leaves connected to them
	for (unsigned i = 0; i < graphPtr->getNumberOfSourceEntities(); ++i) {
		sourcesPriorityQueue.push(pair<unsigned, unsigned>(i, 0));
	}

}

ProvenanceAssignment::~ProvenanceAssignment() {
}

shared_ptr<ProvenanceGraph> ProvenanceAssignment::getGraph() const {
	return graphPtr;
}

unsigned ProvenanceAssignment::getNumberOfAssignedProvenanceIds() const {
	unsigned count = 0;
	for (auto it = seenIds.begin(); it != seenIds.end(); it = seenIds.equal_range(*it).second) {
		++count;
	}

	return count;
}

void ProvenanceAssignment::connectLeafToSources(unsigned leafId, unsigned sourcesPerLeaf) {
#if DEBUG
	cout << "Connecting entity with id " << leafId << " with sources with ids: ";
#endif
	if (graphPtr->getDepth() > 1) {
		if (!allSourcesConnected) {
			// Pick the top N unconnected sources
			for (unsigned i = 0; i < sourcesPerLeaf; ++i) {
				pair<unsigned, unsigned> top = sourcesPriorityQueue.top();
				if (top.second > 0) {
					// This means all sources had been used once
					allSourcesConnected = true;
				}
				top.second = top.second + 1;
				unsigned absoluteSourceIdx = graphPtr->getSourceAbsoluteId(top.first);
#if DEBUG
				cout << absoluteSourceIdx << " ";
#endif
				graphPtr->connectSourceAndLeaf(absoluteSourceIdx, leafId);
				sourcesPriorityQueue.pop();
				sourcesPriorityQueue.push(top);
			}
		} else {
			// Pick random sources
			for (unsigned i = 0; i < sourcesPerLeaf; ++i) {
				unsigned sourceIdx = rand() % graphPtr->getNumberOfSourceEntities();
				unsigned absoluteSourceIdx = graphPtr->getSourceAbsoluteId(sourceIdx);
#if DEBUG
				cout << absoluteSourceIdx << " ";
#endif
				graphPtr->connectSourceAndLeaf(absoluteSourceIdx, leafId);
			}
		}
	}
#if DEBUG
	cout << endl;
#endif

}

} /* namespace provenance */
} /* namespace fourbench */
