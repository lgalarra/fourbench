/*
 * ProvenanceGraph.cpp
 *
 *  Created on: Jul 3, 2017
 *      Author: galarraga
 */

#include <math.h>
#include <ostream>
#include <iostream>

#include "../include/conf/Conf.hpp"
#include "../include/conf/AssignmentDistribution.hpp"
#include "../include/parsing/ParsingStats.hpp"
#include "../include/provenance/ProvenanceGraph.hpp"

namespace fc = fourbench::conf;
namespace fp = fourbench::parsing;

namespace fourbench {
namespace provenance {


ProvenanceGraph::ProvenanceGraph(const fc::ConfValues& values, const fp::ParsingStats& stats) :
	name(values.familyName), provUsed("prov:used"), provWasAttributedTo("prov:wasAttributedTo"),
	provWasGeneratedBy("prov:wasGeneratedBy"), perSubject(values.provenancePerSubject),
	nSourceEntities(values.numberOfSources), nAgents(values.numberOfAgents),
	nLevels(values.metadataDepth), entities2TriplesDistribution(values.distribution),
	nSubjects(stats.numberOfSubjects), nTriples(stats.numberOfTriples) {

	entityLevels = new int[nLevels + 1];
	activityLevels = new int[nLevels + 1];

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
	if (nLevels < 2) {
		nIntermediateEntities = 0;
	} else {
		unsigned activitiesPerLevel = (unsigned) ceil(nActivities / (float)nLevels);
		nIntermediateEntities = nActivities - activitiesPerLevel; // Substract the activities of one level
	}

}

void ProvenanceGraph::computeNumberOfActivities(const fc::ConfValues& values) {
	if (nLevels > 1) {
		nActivities = (unsigned) ceil(values.activitiesEntitiesDensity
				* (nLeafEntities + nSourceEntities));
	} else {
		nActivities = 0;
	}
}

void ProvenanceGraph::assignActivitiesToLevels() {
	// Activities range in the interval [0, nActivities - 1]
	activityLevels[0] = -1; // The base level does not have any activities
	if (nLevels > 1) {
		// Divide the activities in equal chunks
		unsigned activitiesPerLevel = (unsigned) ceil(nActivities / nLevels);
		unsigned coveredActivities = -1;
		unsigned level = 1;
		while (level <= nLevels) {
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
	if (nLevels > 1) {
		// Divide the intermediate entities in chunks
		unsigned shift = nLeafEntities;
		unsigned limit = shift + nIntermediateEntities - 1;
		unsigned entitiesPerLevel = (unsigned) ceil(nIntermediateEntities / (nLevels - 1));
		unsigned coveredIntermediateEntities = shift;
		while (level < nLevels) {
			coveredIntermediateEntities = min(coveredIntermediateEntities + entitiesPerLevel, limit);
			entityLevels[level] = coveredIntermediateEntities;
			++level;
		}

		entityLevels[nLevels] = nEntities - 1;
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
	for (unsigned level = 1; level <= nLevels; ++level) {
		// This means the target entity is the source
		if (level == nLevels) {
			connectEntities(latestEntity, sourceId, level);
		} else {
			// We have to look for intermediate entities in level + 1
			unsigned entityNextLevel = getRandomEntityInLevel(level + 1);
			connectEntities(latestEntity, entityNextLevel, level + 1);
			latestEntity = entityNextLevel;
		}
	}

	cout << ". Connecting source entity with id " << sourceId << " and leaf with id " << leafId << endl;
}

void ProvenanceGraph::connectEntities(unsigned sourceId, unsigned targetId, unsigned targetLevel) {

}

void ProvenanceGraph::getRandomEntityInLevel(unsigned level) const {

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
	return nLevels;
}


} /* namespace provenance */
} /* namespace fourbench */
