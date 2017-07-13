/*
 * UniformProvenanceAssignment.hpp
 *
 *  Created on: Jul 13, 2017
 *      Author: galarraga
 */

#ifndef PROVENANCE_UNIFORMPROVENANCEASSIGNMENT_HPP_
#define PROVENANCE_UNIFORMPROVENANCEASSIGNMENT_HPP_

#include <memory>
#include <queue>
#include <map>

#include "ProvenanceAssignment.hpp"
#include "ProvenanceGraph.hpp"

using namespace std;

namespace fourbench {
namespace provenance {

struct SourceCountLessThanStruct {
	bool operator()(const pair<unsigned, unsigned>& lhs, const pair<unsigned, unsigned>& rhs) const {
		if (lhs.second < rhs.second)
			return true;
		else if (lhs.second > rhs.second)
			return false;
		else
			return lhs.first < rhs.first;
	}
};

typedef struct SourceCountLessThanStruct SourceCountLessThanStruct;

class UniformProvenanceAssignment : public ProvenanceAssignment {
private:
	unsigned sourcesPerLeaf;
	unsigned numberOfSources;
	unsigned numberOfLeaves;
	bool allSourcesConnected;
	priority_queue<pair<unsigned, unsigned>, vector<pair<unsigned, unsigned>>, SourceCountLessThanStruct> sourcesPriorityQueue;
	unsigned latestLeaf;
	unsigned numberOfAssignmentsLatestLeaf;
	unsigned maxItemsPerLeaf;

public:
	UniformProvenanceAssignment(shared_ptr<ProvenanceGraph> graphPtr);
	virtual ~UniformProvenanceAssignment();
	virtual unsigned nextProvenanceId();
};

} /* namespace provenance */
} /* namespace fourbench */

#endif /* PROVENANCE_UNIFORMPROVENANCEASSIGNMENT_HPP_ */
