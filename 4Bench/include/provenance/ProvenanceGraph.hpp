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
#include <set>
#include <vector>

#include "../include/conf/Conf.hpp"
#include "../include/conf/AssignmentDistribution.hpp"
#include "../include/parsing/ParsingStats.hpp"
#include "../include/utils/string.hpp"

#include "Activity.hpp"
#include "Agent.hpp"
#include "Entity.hpp"
#include "ProvenanceSubgraph.hpp"
#include "IRIBuilder.hpp"

namespace fc = fourbench::conf;
namespace fp = fourbench::parsing;
namespace f = fourbench;

using namespace std;

namespace fourbench {
namespace provenance {

class ProvenanceGraph;

template <class Domain>
class NodeIterator {
private:
	vector<unsigned>* usedIds;
protected:
	unsigned cursor;
	ProvenanceGraph *graph;
	NodeIterator(ProvenanceGraph *graph, vector<unsigned> *ids);
	NodeIterator(ProvenanceGraph *graph, vector<unsigned>* ids, int cursor);
public:
	~NodeIterator();
	NodeIterator(const NodeIterator& o);
	NodeIterator& operator=(const NodeIterator& o);
	NodeIterator& operator++();
	shared_ptr<Domain> operator*();
	bool operator==(const NodeIterator& o) const;
	bool operator!=(const NodeIterator& o) const;
	friend class ProvenanceGraph;
};

template <class Domain>
class ImplicitNodeIterator : public NodeIterator<Domain> {
private:
	unsigned maxId;
	ImplicitNodeIterator(unsigned maxId, ProvenanceGraph *graph);
	ImplicitNodeIterator(unsigned maxId, ProvenanceGraph *graph, int cursor);
public:
	shared_ptr<Domain> operator*();
	~ImplicitNodeIterator();
	friend class ProvenanceGraph;
};

template <class Domain, class Range>
class EdgeIterator {
private:
	multimap<unsigned, unsigned>* matrix;
	multimap<unsigned, unsigned>::const_iterator backIterator;
	string property;
	string domain;

	EdgeIterator(multimap<unsigned, unsigned>* matrix, const string& domain, const string& property);
	EdgeIterator(multimap<unsigned, unsigned>* matrix, const string& domain, const string& property,
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
	static string provenanceGraphDefaultIRI;

	// Provenance graph name
	string name;

	// Pointer to an IRI builder
	shared_ptr<IRIBuilder> iriBuilder;

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

	// Maximal number of attributes for objects of type entity, activity or agent
	unsigned maxNAttributes;

	// Total number of agents
	unsigned nAgents;

	// Maximum number of agents assigned to each activity
	unsigned maxNAgentsPerActivity;

	// Maximum number of agents assigned to each source entity
	unsigned maxNAgentsPerSource;

	// Number of levels
	unsigned maxLevel;

	// Levels for entities
	int* entityLevels;

	// Levels for activites
	int* activityLevels;

	// Sources and leaves density
	float sources2LeavesDensity;

	// Triples to leaf entities density (alpha value in proposal: http://wiki.qweb.cs.aau.dk/w/index.php/Benchmark_to_evaluate_quad-stores)
	float triples2EntitiesDensity;

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

	void computeNumberOfLeafEntities(fc::AssignmentDistribution dist, unsigned N);

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

	string toDebugString(const int* levelsMap) const {
		stringstream strm;
		strm << "[";
		for (unsigned level = 0; level <= maxLevel; ++level) {
			strm << level << ": " <<  levelsMap[level] << ", ";
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
		strm << "# triples: " << g.nTriples << ", ";
		strm << "# subjects: " << g.nSubjects << ", ";
		strm << "# (total entities, leaf entities, intermediate entities, sources): ";
		strm << "(" << g.nEntities << ", " << g.nLeafEntities << ", " << g.nIntermediateEntities << ", " << g.nSourceEntities << "), ";
		strm << "entity levels: " << g.toString(g.entityLevels) << ", ";
		strm << "activity levels: " << g.toString(g.activityLevels);
		//strm << ", entity levels (debug): " << g.toDebugString(g.entityLevels) << ", ";
		//strm << "activity levels (debug): " << g.toDebugString(g.activityLevels);
		strm << "}";
		return strm;
	}

	static string getDefaultProvenanceGraphIRI();

	string getName() const;

	string getDomain() const;

	unsigned getNumberOfSourceEntities() const;

	unsigned getNumberOfLeafEntities() const;

	unsigned getNumberOfIntermediateEntities() const;

	unsigned getNumberOfActivities() const;

	unsigned getNumberOfEntities() const;

	unsigned getNumberOfAgents() const;

	float getSources2LeavesDensity() const;

	float getTriples2EntitiesDensity() const;

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

	unsigned getMaxNumberOfAttributes() const;

	EntityLevel getLevelForEntityId(unsigned entityId) const;

	int getLevelValueForEntityId(unsigned entityId) const;

	vector<shared_ptr<Agent>> getAgentsForActivity(const Activity&) const;

	vector<shared_ptr<Agent>> getAgentsForSource(const Entity&) const;

	pair<EdgeIterator<Activity, Entity>, EdgeIterator<Activity, Entity>> getProvUsedIterators();

	pair<EdgeIterator<Entity, Activity>, EdgeIterator<Entity, Activity>> getProvWasGeneratedByIterators();

	pair<EdgeIterator<Entity, Agent>, EdgeIterator<Entity, Agent>> getProvWasAttributedToIterators();

	pair<NodeIterator<Agent>, NodeIterator<Agent>> getAgentIterators();

	pair<NodeIterator<Activity>, NodeIterator<Activity>> getActivityIterators();

	pair<NodeIterator<Entity>, NodeIterator<Entity>> getEntityIterators();

	shared_ptr<vector<unsigned>> getUsedEntityIds() const;

public:
	virtual ~ProvenanceGraph();

	friend class ProvenanceGraphFactory;
};

ostream& operator<<(ostream&, const ProvenanceGraph&);

} /* namespace provenance */
} /* namespace fourbench */

#endif /* PROVENANCE_PROVENANCEGRAPH_HPP_ */
