/*
 * QuadTSVProvenanceDump.cpp
 *
 *  Created on: Jul 19, 2017
 *      Author: galarraga
 */

#include <output/NQuadsProvenanceDump.hpp>
#include <ostream>
#include <tuple>

#include "../include/provenance/Activity.hpp"
#include "../include/provenance/Entity.hpp"
#include "../include/provenance/ProvenanceGraph.hpp"
#include "../include/provenance/PROVO.hpp"
#include "../include/output/ProvenanceDump.hpp"

using namespace std;

namespace fprov = fourbench::provenance;

namespace fourbench {
namespace output {

NQuadsProvenanceDump::NQuadsProvenanceDump(ostream& strm) : ProvenanceDump(strm) {

}

NQuadsProvenanceDump::~NQuadsProvenanceDump() {

}

void NQuadsProvenanceDump::dump(const fprov::ProvenanceObject& obj, string attributeName, shared_ptr<fd::DataValue> attributeValue) {
	this->formatIRI(obj.getIRI());
	stream << " ";
	this->formatIRI(attributeName);
	stream << " ";
	this->format(attributeValue);
	stream << " ";
	this->formatIRI(fprov::ProvenanceGraph::getDefaultProvenanceGraphIRI());
	stream << " .";
	stream << endl;
}

void NQuadsProvenanceDump::dump(const fpar::Triple& triple, const fprov::Entity& provEntity) {
	stream << triple.getSubject();
	stream << " ";
	stream << triple.getPredicate();
	stream << " ";
	stream << triple.getObject();
	stream << " ";
	this->formatIRI(provEntity.getIRI());
	stream << " .";
	stream << endl;
	++triplesDumped;
}

void NQuadsProvenanceDump::dump(const fprov::ProvenanceObject& obj) {
	ProvenanceDump::dump(obj);
}


void NQuadsProvenanceDump::dump(shared_ptr<fprov::ProvenanceGraph> graph) {
#ifdef DEBUG
	cout << "Dumping prov:used relationships between activities and entities." << endl;
#endif
	pair<fprov::EdgeIterator<fprov::Activity, fprov::Entity>, fprov::EdgeIterator<fprov::Activity, fprov::Entity>> provUsedIterators = graph->getProvUsedIterators();
	for (auto it = provUsedIterators.first; it != provUsedIterators.second; ++it) {
		this->dump<fprov::Activity, fprov::Entity>(*it);
	}

#ifdef DEBUG
	cout << "Dumping relationships prov:wasGeneratedBy relationships between entities and activities." << endl;
#endif

	pair<fprov::EdgeIterator<fprov::Entity, fprov::Activity>, fprov::EdgeIterator<fprov::Entity, fprov::Activity>> provWasGeneratedBy = graph->getProvWasGeneratedByIterators();
	for (auto it = provWasGeneratedBy.first; it != provWasGeneratedBy.second; ++it) {
		this->dump<fprov::Entity, fprov::Activity>(*it);
	}

#ifdef DEBUG
	cout << "Dumping relationships prov:wasAssociatedWith relationships between activities and agents." << endl;
#endif

	for (unsigned activityId = 0; activityId < graph->getNumberOfActivities(); ++activityId) {
		fprov::Activity activity(activityId, graph->getDomain());
		// Print all its edges
		this->dump<fprov::Activity, fprov::Agent>(activity, fprov::PROVO::wasAssociatedWith, graph->getAgentsForActivity(activity));
		this->dump(activity);

	}

#ifdef DEBUG
	cout << "Dumping relationships prov:wasAttributedTo relationships between entities and agents." << endl;
#endif

	unsigned firstSource = graph->getFirstSourceId();
	for (unsigned sourceEntityId = firstSource; sourceEntityId < firstSource + graph->getNumberOfSourceEntities(); ++sourceEntityId) {
		fprov::Entity entity(sourceEntityId, graph->getDomain(), graph->getMaxNumberOfAttributes(), graph->getDepth() > 0 ? fprov::EntityLevel::SOURCE : fprov::EntityLevel::SOURCE_AND_LEAF);
		this->dump<fprov::Entity, fprov::Agent>(entity, fprov::PROVO::wasAttributedTo, graph->getAgentsForSource(entity));
		this->dump(entity);
	}

#ifdef DEBUG
	cout << "Dumping agents' attributes" << endl;
#endif
	// Dump the agents
	for (unsigned agentId = 0; agentId < graph->getNumberOfAgents(); ++agentId) {
		fprov::Agent agent(agentId, graph->getDomain(), graph->getMaxNumberOfAttributes());
		this->dump(agent);
	}


	if (graph->getDepth() > 0) {
#ifdef DEBUG
	cout << "Dumping entities' attributes." << endl;
#endif
		shared_ptr<vector<unsigned>> usedEntityIds = graph->getUsedEntityIds();
		for (auto it = usedEntityIds->begin(); it != usedEntityIds->end(); ++it) {
			fprov::Entity entity(*it, graph->getDomain(), graph->getMaxNumberOfAttributes());
			this->dump(entity);
		}
	}
}

} /* namespace output */
} /* namespace fourbench */
