/*
 * ProvenanceAssignment.hpp
 *
 *  Created on: Jul 12, 2017
 *      Author: galarraga
 */

#ifndef PROVENANCE_PROVENANCEASSIGNMENT_HPP_
#define PROVENANCE_PROVENANCEASSIGNMENT_HPP_

#include <memory>
#include <queue>
#include <unordered_set>

#include "ProvenanceGraph.hpp"

using namespace std;

namespace fourbench {
namespace provenance {

struct SourceCountLessThanStruct {
	bool operator()(const pair<unsigned, unsigned>& lhs, const pair<unsigned, unsigned>& rhs) const {
		if (lhs.second > rhs.second)
			return true;
		else if (lhs.second < rhs.second)
			return false;
		else
			return lhs.first > rhs.first;
	}
};

typedef struct SourceCountLessThanStruct SourceCountLessThanStruct;


class ProvenanceAssignment {
protected:
	shared_ptr<ProvenanceGraph> graphPtr;
	unordered_multiset<unsigned> seenIds;
	bool allSourcesConnected;
	unsigned latestLeaf;
	unsigned sourcesPerLeaf;
	priority_queue<pair<unsigned, unsigned>, vector<pair<unsigned, unsigned>>, SourceCountLessThanStruct> sourcesPriorityQueue;
	ProvenanceAssignment(shared_ptr<ProvenanceGraph> graphPtr);
	virtual void connectLeafToSources(unsigned leafId, unsigned numberOfSources);
public:
	virtual ~ProvenanceAssignment();
	virtual unsigned nextProvenanceId() = 0;
	shared_ptr<ProvenanceGraph> getGraph() const;
};

} /* namespace provenance */
} /* namespace fourbench */

#endif /* PROVENANCEASSIGNMENT_HPP_ */
