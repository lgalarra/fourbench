/*
 * ProvenanceGraph.cpp
 *
 *  Created on: Jul 3, 2017
 *      Author: galarraga
 */

#include <math.h>

#include "../include/conf/Conf.hpp"
#include "../include/conf/AssignmentDistribution.hpp"
#include "../include/parsing/ParsingStats.hpp"
#include "../include/provenance/ProvenanceGraph.hpp"

namespace fc = fourbench::conf;
namespace fp = fourbench::parsing;

namespace fourbench {
namespace provenance {

ProvenanceGraph::ProvenanceGraph(const fc::ConfValues& values, const fp::ParsingStats& stats) :
	provUsed("prov:used"), provWasAttributedTo("prov:wasAttributedTo"),
	provWasGeneratedBy("prov:wasGeneratedBy"), perSubject(values.provenancePerSubject),
	nSourceEntities(values.numberOfSources), nAgents(values.numberOfAgents),
	nLevels(values.metadataDepth) {

	if (nSourceEntities == fc::Conf::AUTO) {
		computeNumberOfSourceEntities(values, stats);
	}

	if (this->perSubject) {
		computeNumberOfLeafEntities(values, stats.numberOfSubjects);
	} else {
		computeNumberOfLeafEntities(values, stats.numberOfTriples);
	}

	computeNumberOfActivities(values);
	assignActivitiesToLevels();
	computeNumberOfNonLeafEntities(values);

}

void ProvenanceGraph::computeNumberOfSourceEntities(const fc::ConfValues& values, const fp::ParsingStats& stats) {
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
		float alpha = ((1 - N) / (float)N) * values.agentsEntitiesDensity + 1;
		leafEntities = (unsigned)ceil(alpha * N);
		break;
	}
	/**
	 * For the rest of the cases set the upper limit in the number of leaves.
	 */
	case fc::AssignmentDistribution::POWER_LAW :
		leafEntities = N;
		break;
	case fc::AssignmentDistribution::GEOMETRIC :
		leafEntities = N;
		break;
	}
}

void ProvenanceGraph::computeNumberOfNonLeafEntities(const fc::ConfValues& values) {
	if (nLevels < 2) {
		nIntermediateEntities = 0;
	} else {
		nIntermediateEntities = (unsigned)ceil(nActivities / 2.0f);
	}

}

void ProvenanceGraph::computeNumberOfActivities(const fc::ConfValues& values) {
	if (nLevels > 1) {
		nActivities = (unsigned) ceil(values.activitiesEntitiesDensity
				* (leafEntities + nSourceEntities));
	} else {
		nActivities = 0;
	}
}

void ProvenanceGraph::assignActivitiesToLevels() {
	if (nLevels > 1 && nActivities > 0) {
		// Divide the activities in equal chunks
		unsigned activitiesPerLevel = (unsigned) ceil(nActivities / nLevels);
		unsigned coveredActivities = 0;
		unsigned level = 0;
		while (coveredActivities < nActivities) {
			++level;
			coveredActivities += activitiesPerLevel;
			activityLevels[level] = coveredActivities;
		}
	}
}

ProvenanceGraph::~ProvenanceGraph() {
	// TODO Auto-generated destructor stub
}


} /* namespace provenance */
} /* namespace fourbench */
