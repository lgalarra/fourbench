/*
 * ProvenanceGraph.hpp
 *
 *  Created on: Jul 3, 2017
 *      Author: galarraga
 */

#ifndef PROVENANCE_PROVENANCEGRAPH_HPP_
#define PROVENANCE_PROVENANCEGRAPH_HPP_


namespace fc = fourbench::conf;

namespace fourbench {
namespace provenance {

class ProvenanceGraph {
private:
	// Total number of provenance entities
	unsigned nEntities;

	// Total number of leaf entities
	unsigned leafEntities;

	// Total number of activities
	unsigned nActivities;

	// Total number of agents
	unsigned nAgents;

	// Levels for entities
	map<unsigned, unsigned> entityLevels;

	// Levels for activites
	map<unsigned, unsigned> activityLevels;

	// Adjacency matrix for prov:used relation (from activities to entities)
	ProvenanceSubgraph provUsed;

	// Adjacency matrix for prov:wasGeneratedBy relation (from entities to activities)
	ProvenanceSubgraph provWasGeneratedBy;

	// Adjacency matrix for prov:wasAttributedTo relation (from entities to agents)
	ProvenanceSubgraph provWasAttributedTo;

	ProvenanceGraph(const fc::ConfValues& values, unsigned nTriples);

	void computeNumberOfLeafEntities(const fc::ConfValues& values, unsigned nTriples);

public:
	virtual ~ProvenanceGraph();

	friend class ProvenanceGraphBuilder;
};

} /* namespace provenance */
} /* namespace fourbench */

#endif /* PROVENANCE_PROVENANCEGRAPH_HPP_ */
