/*
 * ProvenanceGraph.cpp
 *
 *  Created on: Jul 3, 2017
 *      Author: galarraga
 */

#include <stdlib.h>
#include <math.h>
#include <ostream>
#include <iostream>
#include <map>
#include <tuple>
#include <iterator>
#include <memory>
#include <set>
#include <vector>

#include "../include/utils/integer.hpp"
#include "../include/utils/string.hpp"
#include "../include/conf/Conf.hpp"
#include "../include/conf/AssignmentDistribution.hpp"
#include "../include/parsing/ParsingStats.hpp"
#include "../include/provenance/IRIBuilder.hpp"
#include "../include/provenance/ProvenanceGraph.hpp"
#include "../include/provenance/PROVO.hpp"
#include "../include/provenance/Activity.hpp"
#include "../include/provenance/Entity.hpp"
#include "../include/provenance/Agent.hpp"

namespace fc = fourbench::conf;
namespace fp = fourbench::parsing;
namespace f = fourbench;

namespace fourbench {
namespace provenance {

template<class Domain>
NodeIterator<Domain>::NodeIterator(vector<unsigned>* ids, fc::ConfValues* confVal, IRIBuilder* iriBuilder) :
	cursor(0), conf(confVal), ids(ids), iriBuilder(iriBuilder) {

}

template<class Domain>
NodeIterator<Domain>::NodeIterator(vector<unsigned>* ids, fc::ConfValues* confVal, IRIBuilder* iriBuilder, int cursor) :
	conf(confVal), ids(ids), iriBuilder(iriBuilder), cursor(cursor) {

}

template <class Domain>
NodeIterator<Domain>::NodeIterator(const NodeIterator& o) :
	cursor(o.cursor), conf(o.conf), ids(o.ids), iriBuilder(o.iriBuilder) {

}

template <class Domain>
NodeIterator<Domain>& NodeIterator<Domain>::operator=(const NodeIterator& o) {
	cursor = o.cursor;
	conf = o.conf;
	ids = o.ids;
	iriBuilder = o.iriBuilder;
}

template <class Domain>
NodeIterator<Domain>& NodeIterator<Domain>::operator++() {
	++cursor;
	return *this;
}

template <class Domain>
shared_ptr<Domain> NodeIterator<Domain>::operator*() {
	shared_ptr<Domain> objectPtr = make_shared<Domain>(ids->at(cursor), iriBuilder->getDomain());
	objectPtr->populateWithAttributes(*conf);
	return objectPtr;
}

template <class Domain>
bool NodeIterator<Domain>::operator==(const NodeIterator& o) const {
	return ids == o.ids && cursor == o.cursor && conf == o.conf && iriBuilder == o.iriBuilder;
}

template <class Domain>
bool NodeIterator<Domain>::operator!=(const NodeIterator& o) const {
	return !(*this == o);
}

template <class Domain>
ImplicitNodeIterator<Domain>::ImplicitNodeIterator(unsigned maxId, fc::ConfValues* confVal, IRIBuilder* iriBuilder)
	: NodeIterator<Domain>(nullptr, confVal, iriBuilder), maxId(maxId) {

}

template <class Domain>
ImplicitNodeIterator<Domain>::ImplicitNodeIterator(unsigned maxId, fc::ConfValues* confVal, IRIBuilder* iriBuilder, int cursor)
	: NodeIterator<Domain>(nullptr, confVal, iriBuilder, cursor), maxId(maxId){

}


template <class Domain>
NodeIterator<Domain>::~NodeIterator() {

}


template <class Domain>
ImplicitNodeIterator<Domain>::~ImplicitNodeIterator() {

}

template <class Domain>
shared_ptr<Domain> ImplicitNodeIterator<Domain>::operator*() {
	shared_ptr<Domain> objectPtr = make_shared<Domain>(this->cursor, this->iriBuilder->getDomain());
	objectPtr->populateWithAttributes(*this->conf);
	return objectPtr;
}


template class NodeIterator<Agent>;
template class NodeIterator<Entity>;
template class NodeIterator<Activity>;
template class ImplicitNodeIterator<Agent>;
template class ImplicitNodeIterator<Entity>;
template class ImplicitNodeIterator<Activity>;

template <class Domain, class Range>
EdgeIterator<Domain, Range>::EdgeIterator(multimap<unsigned, unsigned>* matrix,
		const string& domain, const string& property): matrix(matrix), domain(domain),
		backIterator(matrix->cbegin()), property(property) {
}

template <class Domain, class Range>
EdgeIterator<Domain, Range>::EdgeIterator(multimap<unsigned, unsigned>* matrix,
		const string& domain, const string& property, multimap<unsigned, unsigned>::const_iterator it) : matrix(matrix),
		domain(domain), backIterator(it), property(property) {
}

template <class Domain, class Range>
EdgeIterator<Domain, Range>::~EdgeIterator() {

}

template <class Domain, class Range>
EdgeIterator<Domain, Range>::EdgeIterator(const EdgeIterator& o) : matrix(o.matrix), domain(o.domain),
		backIterator(o.backIterator), property(o.property) {
}

template <class Domain, class Range>
EdgeIterator<Domain, Range>& EdgeIterator<Domain, Range>::operator=(const EdgeIterator& o) {
	matrix = o.matrix;
	domain = o.domain;
	backIterator = o.backIterator;
	property = o.property;
	return *this;
}

template <class Domain, class Range>
EdgeIterator<Domain, Range>& EdgeIterator<Domain, Range>::operator++() {
	++backIterator;
	return *this;

}

template <class Domain, class Range>
tuple<shared_ptr<Domain>, string, shared_ptr<Range>> EdgeIterator<Domain, Range>::operator*() {
	const pair<unsigned, unsigned>& pair = *backIterator;
	return make_tuple(make_shared<Domain>(pair.first, domain), property, make_shared<Range>(pair.second, domain));
}

template <class Domain, class Range>
bool EdgeIterator<Domain, Range>::operator==(const EdgeIterator& o) const {
	return matrix == o.matrix && domain == o.domain && backIterator == o.backIterator && property == o.property;
}

template <class Domain, class Range>
bool EdgeIterator<Domain, Range>::operator!=(const EdgeIterator& o) const {
	return !(*this == o);
}

template class EdgeIterator<Activity, Entity>;
template class EdgeIterator<Entity, Activity>;

string ProvenanceGraph::provenanceGraphDefaultIRI = f::concat({IRIBuilder::getDefaultDomain(), "/graph/"});

string ProvenanceGraph::getDefaultProvenanceGraphIRI() {
	return provenanceGraphDefaultIRI;
}

ProvenanceGraph::ProvenanceGraph(const fc::ConfValues& values, const fp::ParsingStats& stats) :
	name(values.familyName), provUsed(PROVO::used), provWasAttributedTo(PROVO::wasAttributedTo),
	provWasGeneratedBy(PROVO::wasGeneratedBy), perSubject(values.provenancePerSubject),
	nSourceEntities(values.numberOfSources), nAgents(values.numberOfAgents),
	maxNAgentsPerActivity(values.maxNumberOfAgentsPerActivity), maxNAgentsPerSource(values.maxNumberOfAgentsPerSourceEntity),
	maxLevel(values.metadataDepth), entities2TriplesDistribution(values.distribution),
	nSubjects(stats.numberOfSubjects), nTriples(stats.numberOfTriples) {

	entityLevels = new int[maxLevel + 1];
	activityLevels = new int[maxLevel + 1];

	if (nSourceEntities == fc::Conf::AUTO) {
		computeNumberOfSourceEntities(values, stats);
	}

	if (this->perSubject) {
		computeNumberOfLeafEntities(values, stats.numberOfSubjects);
	} else {
		computeNumberOfLeafEntities(values, stats.numberOfTriples);
	}

	computeNumberOfActivities(values); // This requires number of source and leaf entities
	computeNumberOfIntermediateEntities(values); // This requires number of activities
	computeTotalNumberOfEntities();

	assignActivitiesToLevels();
	assignEntitiesToLevels();

	iriBuilder = IRIBuilder::getInstance(f::concat({IRIBuilder::getDefaultDomain(), name, "/"}));
}

ProvenanceGraph::~ProvenanceGraph() {
	delete entityLevels;
	delete activityLevels;
}

void ProvenanceGraph::computeTotalNumberOfEntities() {
	nEntities = nSourceEntities + nLeafEntities + nIntermediateEntities;
}

void ProvenanceGraph::computeNumberOfSourceEntities(const fc::ConfValues& values,
		const fp::ParsingStats& stats) {
	switch(values.sourcesDefinition) {
	case fc::SourcesDefinition::PERFILE:
		nSourceEntities = stats.numberOfInputFiles;
		break;
	case fc::SourcesDefinition::PERPROPERTY:
		nSourceEntities = values.properties.size() > 0 ?
				values.properties.size() : stats.numberOfProperties;
		break;
	case fc::SourcesDefinition::PERCLASS:
		nSourceEntities = stats.numberOfClasses;
		break;
	}
}

void ProvenanceGraph::computeNumberOfLeafEntities(const fc::ConfValues& values, unsigned N) {
	const fc::Conf& conf = fc::Conf::defaultConfig();
	switch (values.distribution) {
	case fc::AssignmentDistribution::UNIFORM :
	{
		nLeafEntities = (unsigned)round(N - values.triplesEntitiesDensity * (N - 1));
		break;
	}
	/**
	 * For the rest of the cases set the upper limit in the number of leaves.
	 */
	case fc::AssignmentDistribution::POWER_LAW :
		nLeafEntities = N;
		break;
	case fc::AssignmentDistribution::GEOMETRIC :
		nLeafEntities = N;
		break;
	}
}

void ProvenanceGraph::computeNumberOfIntermediateEntities(const fc::ConfValues& values) {
	if (maxLevel < 2) {
		nIntermediateEntities = 0;
	} else {
		unsigned activitiesPerLevel = (unsigned) ceil(nActivities / (float)maxLevel);
		nIntermediateEntities = nActivities - activitiesPerLevel; // Substract the activities of one level
	}

}

void ProvenanceGraph::computeNumberOfActivities(const fc::ConfValues& values) {
	if (maxLevel > 1) {
		nActivities = (unsigned) ceil(values.activitiesEntitiesDensity
				* (nLeafEntities + nSourceEntities));
	} else {
		nActivities = 0;
	}
}

void ProvenanceGraph::assignActivitiesToLevels() {
	// Activities range in the interval [0, nActivities - 1]
	activityLevels[0] = -1; // The base level does not have any activities
	if (maxLevel > 1) {
		// Divide the activities in equal chunks
		unsigned activitiesPerLevel = (unsigned) ceil(nActivities / maxLevel);
		unsigned coveredActivities = -1;
		unsigned level = 1;
		while (level <= maxLevel) {
			coveredActivities = min(coveredActivities + activitiesPerLevel, nActivities - 1);
			activityLevels[level] = coveredActivities;
			++level;
		}
	}
}

void ProvenanceGraph::assignEntitiesToLevels() {
	// We assume leaf entities are in the interval [0, nLeafEntities - 1]
	// Intermediate entities are in the interval [nLeafEntities, nLeafEntities + nIntermediateEntities - 1]
	// Sources are in the interval [nLeafEntities + nIntermediateEntities, nLeafEntities + nIntermediateEntities + nSourceEntities - 1]
	unsigned level = 0;
	entityLevels[level] = nLeafEntities - 1;
	level = 1;
	if (maxLevel > 1) {
		// Divide the intermediate entities in chunks
		unsigned shift = nLeafEntities;
		unsigned limit = shift + nIntermediateEntities - 1;
		unsigned entitiesPerLevel = (unsigned) ceil(nIntermediateEntities / (maxLevel - 1));
		unsigned coveredIntermediateEntities = shift;
		while (level < maxLevel) {
			coveredIntermediateEntities = min(coveredIntermediateEntities + entitiesPerLevel, limit);
			entityLevels[level] = coveredIntermediateEntities;
			++level;
		}

		entityLevels[maxLevel] = nEntities - 1;
	}
}

string ProvenanceGraph::getName() const {
	return name;
}

string ProvenanceGraph::getDomain() const {
	return iriBuilder->getDomain();
}

unsigned ProvenanceGraph::getNumberOfSourceEntities() const {
	return nSourceEntities;
}

unsigned ProvenanceGraph::getNumberOfLeafEntities() const {
	return nLeafEntities;
}

unsigned ProvenanceGraph::getNumberOfIntermediateEntities() const {
	return nIntermediateEntities;
}

unsigned ProvenanceGraph::getNumberOfActivities() const {
	return nActivities;
}

unsigned ProvenanceGraph::getNumberOfEntities() const {
	return nEntities;
}

fc::AssignmentDistribution ProvenanceGraph::getEntitiesToTriplesDistribution() const {
	return entities2TriplesDistribution;
}

float ProvenanceGraph::getSources2LeavesDensity() const {
	return sources2LeavesDensity;
}

void ProvenanceGraph::connectSourceAndLeaf(unsigned sourceId, unsigned leafId) {
	unsigned latestEntity = leafId;
	for (unsigned level = 0; level <= maxLevel; ++level) {
		// This means the target entity is the source
		if (level == maxLevel) {
			connectEntities(latestEntity, sourceId, level);
		} else {
			// We have to look for intermediate entities in level + 1
			unsigned entityNextLevel = getRandomEntityInLevel(level + 1);
			connectEntities(latestEntity, entityNextLevel, level + 1);
			latestEntity = entityNextLevel;
		}
	}
}

void ProvenanceGraph::connectEntities(unsigned sourceId, unsigned targetId, unsigned targetLevel) {

	int randomActivity = getRandomActivityInLevel(targetLevel);
	cout << "connectEntities(" << sourceId << ", " << targetId << ", " << targetLevel << ") uses random activity " << randomActivity << endl;
	if (randomActivity >= 0) {
		// Add it to the sparse matrix
		provWasGeneratedBy.addEdge(sourceId, randomActivity);
		provUsed.addEdge(randomActivity, targetId);
	}
}

int ProvenanceGraph::getNumberOfActivitiesInLevel(unsigned level) const {
	if (level == 0) {
		return 0;
	} else if (level > maxLevel) {
		return -1;
	} else {
		return activityLevels[level] - activityLevels[level - 1];
	}
}

int ProvenanceGraph::getRandomActivityInLevel(unsigned level) const {
	int firstActivityInLevel = getFirstActivityInLevel(level);
	if (firstActivityInLevel >= 0) {
		unsigned numberOfActivitiesInLevel = getNumberOfActivitiesInLevel(level);
		return (int) f::urand(firstActivityInLevel, firstActivityInLevel + numberOfActivitiesInLevel);
	} else {
		return -1;
	}
}

int ProvenanceGraph::getFirstActivityInLevel(unsigned level) const {
	if (getNumberOfActivitiesInLevel(level) > 0) {
		return activityLevels[level];
	} else {
		return -1;
	}
}

int ProvenanceGraph::getRandomEntityInLevel(unsigned level) const {
	if (level <= maxLevel) {
		unsigned firstEntityInLevel = getFirstEntityIdInLevel(level);
		unsigned numberOfEntitiesInLevel = getNumberOfEntitiesInLevel(level);
		return (int) f::urand(firstEntityInLevel, firstEntityInLevel + numberOfEntitiesInLevel);
	} else {
		return -1;
	}
}

int ProvenanceGraph::getFirstEntityIdInLevel(unsigned level) const {
	if (getNumberOfEntitiesInLevel(level) > 0) {
		return entityLevels[level];
	} else {
		return -1;
	}
}

int ProvenanceGraph::getNumberOfEntitiesInLevel(unsigned level) const {
	if (level <= maxLevel) {
		if (level == 0) {
			return entityLevels[level];
		} else {
			return entityLevels[level] - entityLevels[level - 1];
		}
	} else {
		return -1;
	}
}

unsigned ProvenanceGraph::getSourceAbsoluteId(unsigned sourceIdx) const {
	return getFirstSourceId() + sourceIdx;
}

unsigned ProvenanceGraph::getFirstLeafId() const {
	return 0;
}

unsigned ProvenanceGraph::getFirstSourceId() const {
	return nLeafEntities + nIntermediateEntities;
}

bool ProvenanceGraph::isAssignmentPerSubject() const {
	return perSubject;
}

unsigned ProvenanceGraph::getNumberOfSubjects() const {
	return nSubjects;
}

unsigned ProvenanceGraph::getNumberOfTriples() const {
	return nTriples;
}

unsigned ProvenanceGraph::getDepth() const {
	return maxLevel;
}

vector<shared_ptr<Agent>> ProvenanceGraph::getAgentsForActivity(const Activity& activity) const {
	vector<shared_ptr<Agent>> result;
	srand(activity.getId()); // I want to guarantee that an activity always gets the same random sequence of agents
	for (unsigned i = 0; i < maxNAgentsPerActivity; ++i) {
		result.push_back(make_shared<Agent>(f::urand(0, nAgents), getDomain()));
	}
	return result;
}

vector<shared_ptr<Agent>> ProvenanceGraph::getAgentsForSource(const Entity& sourceEntity) const {
	vector<shared_ptr<Agent>> result;
	srand(sourceEntity.getId());
	for (unsigned i = 0; i < maxNAgentsPerSource; ++i) {
		result.push_back(make_shared<Agent>(f::urand(0, nAgents), getDomain()));
	}

	return result;
}

pair<EdgeIterator<Activity, Entity>, EdgeIterator<Activity, Entity>> ProvenanceGraph::getProvUsedIterators() {
	EdgeIterator<Activity, Entity> begin(&provUsed.matrix, getDomain(), PROVO::used);
	EdgeIterator<Activity, Entity> end(&provUsed.matrix, getDomain(), PROVO::used, provUsed.matrix.cend());
	return make_pair(begin, end);
}

pair<EdgeIterator<Entity, Activity>, EdgeIterator<Entity, Activity>> ProvenanceGraph::getProvWasGeneratedByIterators() {
	EdgeIterator<Entity, Activity> begin(&provWasGeneratedBy.matrix, getDomain(), PROVO::wasGeneratedBy);
	EdgeIterator<Entity, Activity> end(&provWasGeneratedBy.matrix, getDomain(), PROVO::wasGeneratedBy, provWasGeneratedBy.matrix.cend());
	return make_pair(begin, end);
}

pair<EdgeIterator<Entity, Agent>, EdgeIterator<Entity, Agent>> ProvenanceGraph::getProvWasAttributedToIterators() {
	EdgeIterator<Entity, Agent> begin(&provWasAttributedTo.matrix, getDomain(), PROVO::wasAttributedTo);
	EdgeIterator<Entity, Agent> end(&provWasAttributedTo.matrix, getDomain(), PROVO::wasAttributedTo, provWasAttributedTo.matrix.cend());
	return make_pair(begin, end);
}

pair<NodeIterator<Agent>, NodeIterator<Agent>> ProvenanceGraph::getAgentIterators() {
	fc::ConfValues& confVal = fc::Conf::defaultConfig().get(this->name);
	ImplicitNodeIterator<Agent> begin(this->nAgents, &confVal, this->iriBuilder.get());
	ImplicitNodeIterator<Agent> end(this->nAgents, &confVal, this->iriBuilder.get(), this->nAgents);
	return make_pair(begin, end);
}

pair<NodeIterator<Activity>, NodeIterator<Activity>> ProvenanceGraph::getActivityIterators() {
	fc::ConfValues& confVal = fc::Conf::defaultConfig().get(this->name);
	ImplicitNodeIterator<Activity> begin(this->nActivities, &confVal, this->iriBuilder.get());
	ImplicitNodeIterator<Activity> end(this->nActivities, &confVal, this->iriBuilder.get(), this->nActivities);
	return make_pair(begin, end);
}

pair<NodeIterator<Entity>, NodeIterator<Entity>> ProvenanceGraph::getEntityIterators() {

}


} /* namespace provenance */
} /* namespace fourbench */
