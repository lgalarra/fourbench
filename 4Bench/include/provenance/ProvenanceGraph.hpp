/*
 * ProvenanceGraph.hpp
 *
 *  Created on: Jul 3, 2017
 *      Author: galarraga
 */

#ifndef PROVENANCE_PROVENANCEGRAPH_HPP_
#define PROVENANCE_PROVENANCEGRAPH_HPP_

#include "../include/parsing/ParsingStats.hpp"
#include "ProvenanceSubgraph.hpp"

namespace fc = fourbench::conf;
namespace fp = fourbench::parsing;

namespace fourbench {
namespace provenance {

class ProvenanceGraph {
private:
	// Assignments of leaf to triples
	bool perSubject; // If true the unit becomes the subject and not the triple

	// Total number of provenance entities
	unsigned nEntities;

	// Total number of leaf entities
	unsigned leafEntities;

	// Total number of sources
	unsigned nSourceEntities;

	// Total number of intermediate entities
	unsigned nIntermediateEntities;

	// Total number of activities
	unsigned nActivities;

	// Total number of agents
	unsigned nAgents;

	// Number of levels
	unsigned nLevels;

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

	ProvenanceGraph(const fc::ConfValues& values, const fp::ParsingStats& stats);

	void computeNumberOfSourceEntities(const fc::ConfValues& values,
			const fp::ParsingStats& stats);

	void computeNumberOfLeafEntities(const fc::ConfValues& values, unsigned N);

	void computeNumberOfNonLeafEntities(const fc::ConfValues& values);

	void computeNumberOfActivities(const fc::ConfValues& values);

	void assignActivitiesToLevels();


public:
	virtual ~ProvenanceGraph();

	friend class ProvenanceGraphBuilder;
};

} /* namespace provenance */
} /* namespace fourbench */

#endif /* PROVENANCE_PROVENANCEGRAPH_HPP_ */
