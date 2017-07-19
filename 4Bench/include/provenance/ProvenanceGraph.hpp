/*
 * ProvenanceGraph.hpp
 *
 *  Created on: Jul 3, 2017
 *      Author: galarraga
 */

#ifndef PROVENANCE_PROVENANCEGRAPH_HPP_
#define PROVENANCE_PROVENANCEGRAPH_HPP_

#include <ostream>
#include <iterator>
#include <memory>
#include <utility>

#include "../include/conf/Conf.hpp"
#include "../include/conf/AssignmentDistribution.hpp"
#include "../include/parsing/ParsingStats.hpp"
#include "../include/provenance/Activity.hpp"
#include "../include/provenance/Entity.hpp"
#include "ProvenanceSubgraph.hpp"

namespace fc = fourbench::conf;
namespace fp = fourbench::parsing;

using namespace std;

namespace fourbench {
namespace provenance {

template <class Domain, class Range>
class EdgeIterator {
private:
	multimap<unsigned, unsigned>* matrix;
	multimap<unsigned, unsigned>::const_iterator backIterator;
	string property;

	EdgeIterator(multimap<unsigned, unsigned>* matrix, const string& property);
	EdgeIterator(multimap<unsigned, unsigned>* matrix, const string& property,
			multimap<unsigned, unsigned>::const_iterator it);

public:
	~EdgeIterator();
	EdgeIterator(const EdgeIterator& o);                   // Copy constructor
	EdgeIterator& operator=(const EdgeIterator& o);        // Assignment operator
	EdgeIterator& operator++();                   // Next element
	tuple<shared_ptr<Domain>, string, shared_ptr<Range>> operator*();                    // Dereference
	bool operator==(const EdgeIterator& o) const; // Comparison
	bool operator!=(const EdgeIterator& o) const;
	friend class ProvenanceGraph;
};

class ProvenanceGraph {
private:
	// Provenance graph name
	string name;

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
	unsigned maxLevel;

	// Levels for entities
	int* entityLevels;

	// Levels for activites
	int* activityLevels;

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

	void connectEntities(unsigned sourceId, unsigned targetId, unsigned targetLevel);

	/**
	 * It returns -1 if it cannot find any entity in the level.
	 */
	int getRandomEntityInLevel(unsigned level) const;


	string toString(const int* levelsMap) const {
		stringstream strm;
		strm << "[";
		int start = 0;
		int end;
		for (unsigned level = 0; level <= maxLevel; ++level) {
			end = levelsMap[level];
			// If there are no items in that level, no need to write anything
			if (end >= start) {
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
		strm << "name: " << g.name << ", ";
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

	unsigned getDepth() const;

	int getFirstEntityIdInLevel(unsigned level) const;

	int getNumberOfEntitiesInLevel(unsigned level) const;

	int getNumberOfActivitiesInLevel(unsigned level) const;

	int getRandomActivityInLevel(unsigned level) const;

	int getFirstActivityInLevel(unsigned level) const;

	pair<EdgeIterator<Activity, Entity>, EdgeIterator<Activity, Entity>> getProvUsedIterators();

public:
	virtual ~ProvenanceGraph();

	friend class ProvenanceGraphFactory;
};

ostream& operator<<(ostream&, const ProvenanceGraph&);

} /* namespace provenance */
} /* namespace fourbench */

#endif /* PROVENANCE_PROVENANCEGRAPH_HPP_ */
