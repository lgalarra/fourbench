/*
 * ProvenanceGraph.cpp
 *
 *  Created on: Jul 3, 2017
 *      Author: galarraga
 */

#include <math.h>
#include <ostream>
#include <iostream>
#include <map>
#include <tuple>
#include <iterator>

#include "../include/utils/integer.hpp"
#include "../include/conf/Conf.hpp"
#include "../include/conf/AssignmentDistribution.hpp"
#include "../include/parsing/ParsingStats.hpp"
#include "../include/provenance/ProvenanceGraph.hpp"
#include "../include/provenance/Activity.hpp"
#include "../include/provenance/Entity.hpp"

namespace fc = fourbench::conf;
namespace fp = fourbench::parsing;
namespace f = fourbench;

namespace fourbench {
namespace provenance {

template <class Domain, class Range>
EdgeIterator<Domain, Range>::EdgeIterator(multimap<unsigned, unsigned>* matrix,
		const string& property): matrix(matrix), backIterator(matrix->cbegin()),
				property(property) {
}

template <class Domain, class Range>
EdgeIterator<Domain, Range>::EdgeIterator(multimap<unsigned, unsigned>* matrix,
		const string& property, multimap<unsigned, unsigned>::const_iterator it) : matrix(matrix),
				backIterator(it), property(property) {

}

template <class Domain, class Range>
EdgeIterator<Domain, Range>::~EdgeIterator() {

}

template <class Domain, class Range>
EdgeIterator<Domain, Range>::EdgeIterator(const EdgeIterator& o) : matrix(o.matrix),
		backIterator(o.backIterator), property(property) {

}

template <class Domain, class Range>
EdgeIterator<Domain, Range>& EdgeIterator<Domain, Range>::operator=(const EdgeIterator& o) {
	matrix = o.matrix;
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
	return make_tuple(make_shared<Domain>(pair.first), property, make_shared<Range>(pair.second));
}

template <class Domain, class Range>
bool EdgeIterator<Domain, Range>::operator==(const EdgeIterator& o) const {
	return matrix == o.matrix && backIterator == o.backIterator && property == property;
}

template <class Domain, class Range>
bool EdgeIterator<Domain, Range>::operator!=(const EdgeIterator& o) const {
	return !(*this == o);
}

template class EdgeIterator<Activity, Entity>;


ProvenanceGraph::ProvenanceGraph(const fc::ConfValues& values, const fp::ParsingStats& stats) :
	name(values.familyName), provUsed("prov:used"), provWasAttributedTo("prov:wasAttributedTo"),
	provWasGeneratedBy("prov:wasGeneratedBy"), perSubject(values.provenancePerSubject),
	nSourceEntities(values.numberOfSources), nAgents(values.numberOfAgents),
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
	for (unsigned level = 1; level <= maxLevel; ++level) {
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
	if (randomActivity >= 0) {
		// Add it to the sparse matrix
		provWasGeneratedBy.addEdge(randomActivity, sourceId);
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

pair<EdgeIterator<Activity, Entity>, EdgeIterator<Activity, Entity>> ProvenanceGraph::getProvUsedIterators() {
	EdgeIterator<Activity, Entity> begin(&provUsed.matrix, "prov:used");
	EdgeIterator<Activity, Entity> end(&provUsed.matrix, "prov:used", provUsed.matrix.cend());
	return make_pair(begin, end);
}


} /* namespace provenance */
} /* namespace fourbench */
