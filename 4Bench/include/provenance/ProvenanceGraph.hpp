/*
 * ProvenanceGraph.hpp
 *
 *  Created on: Jul 3, 2017
 *      Author: galarraga
 */

#ifndef PROVENANCE_PROVENANCEGRAPH_HPP_
#define PROVENANCE_PROVENANCEGRAPH_HPP_

#include <ostream>

#include "../include/conf/Conf.hpp"
#include "../include/conf/AssignmentDistribution.hpp"
#include "../include/parsing/ParsingStats.hpp"
#include "ProvenanceSubgraph.hpp"

namespace fc = fourbench::conf;
namespace fp = fourbench::parsing;

namespace fourbench {
namespace provenance {

class ProvenanceGraph {
private:
	// Number of distinct subjects described by the provenance graph
	unsigned nSubjects;

	// Number of RDF triples described by the provenance graph
	unsigned nTriples;

	// Assignments of leaf to triples
	bool perSubject; // If true the unit becomes the subject and not the triple

	// Total number of provenance entities
	unsigned nEntities;

	// Total number of leaf entities
	unsigned nLeafEntities;

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
	unsigned* entityLevels;

	// Levels for activites
	unsigned* activityLevels;

	// Sources and leaves density
	float sources2LeavesDensity;

	// Distribution
	fc::AssignmentDistribution entities2TriplesDistribution;

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

	void computeNumberOfIntermediateEntities(const fc::ConfValues& values);

	void computeNumberOfActivities(const fc::ConfValues& values);

	void assignActivitiesToLevels();

	void computeTotalNumberOfEntities();

	void assignEntitiesToLevels();


	string toString(const unsigned* levelsMap) const {
		stringstream strm;
		strm << "[";
		unsigned start = 0;
		unsigned end = 0;
		for (unsigned level = 0; level <= nLevels; ++level) {
			end = levelsMap[level];
			// If there are no items in that level, no need to write anything
			if (end > start) {
				strm << level << ": [" << start << ", " << end << "] ";
				start = end + 1;
			}
		}
		strm << "]";
		return strm.str();
	}

public:
	friend ostream& operator<<(ostream &strm, const ProvenanceGraph& g) {
		strm << "Provenance graph: { ";
		strm << "# agents: " << g.nAgents << ", ";
		strm << "# activities: " << g.nActivities << ", ";
		strm << "# (total entities, leaf entities, intermediate entities, sources): ";
		strm << "(" << g.nEntities << ", " << g.nLeafEntities << ", " << g.nIntermediateEntities << ", " << g.nSourceEntities << "), ";
		strm << "entity levels: " << g.toString(g.entityLevels) << ", ";
		strm << "activity levels: " << g.toString(g.activityLevels);
		strm << "}";
		return strm;
	}

	unsigned getNumberOfSourceEntities() const;

	unsigned getNumberOfLeafEntities() const;

	unsigned getNumberOfIntermediateEntities() const;

	unsigned getNumberOfActivities() const;

	unsigned getNumberOfEntities() const;

	float getSources2LeavesDensity() const;

	fc::AssignmentDistribution getEntitiesToTriplesDistribution() const;

	void connectSourceAndLeaf(unsigned sourceId, unsigned leafId);

	unsigned getSourceAbsoluteId(unsigned sourceIdx) const;

	unsigned getFirstLeafId() const;

	bool isAssignmentPerSubject() const;

	unsigned getNumberOfSubjects() const;

	unsigned getNumberOfTriples() const;

	unsigned getFirstSourceId() const;


public:
	virtual ~ProvenanceGraph();

	friend class ProvenanceGraphFactory;
};

ostream& operator<<(ostream&, const ProvenanceGraph&);

} /* namespace provenance */
} /* namespace fourbench */

#endif /* PROVENANCE_PROVENANCEGRAPH_HPP_ */
