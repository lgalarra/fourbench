/*
 * QuadTSVProvenanceDump.cpp
 *
 *  Created on: Jul 19, 2017
 *      Author: galarraga
 */

#include <ostream>
#include <tuple>

#include "../include/provenance/Activity.hpp"
#include "../include/provenance/Entity.hpp"
#include "../include/provenance/ProvenanceGraph.hpp"
#include "../include/provenance/PROVO.hpp"
#include "../include/output/QuadTSVProvenanceDump.hpp"
#include "../include/output/ProvenanceDump.hpp"

using namespace std;

namespace fprov = fourbench::provenance;

namespace fourbench {
namespace output {

QuadTSVProvenanceDump::QuadTSVProvenanceDump(ostream& strm) : ProvenanceDump(strm) {

}

QuadTSVProvenanceDump::~QuadTSVProvenanceDump() {

}


void QuadTSVProvenanceDump::dump(const fpar::Triple& triple, const fprov::Entity& provEntity) const {
	this->formatIRI(triple.getSubject());
	stream << "\t";
	this->formatIRI(triple.getPredicate());
	stream << "\t";
	this->formatIRI(triple.getObject());
	stream << "\t";
	this->formatIRI(provEntity.getIRI());
	stream << endl;
}


void QuadTSVProvenanceDump::dump(fprov::ProvenanceGraph& graph) const {
	pair<fprov::EdgeIterator<fprov::Activity, fprov::Entity>, fprov::EdgeIterator<fprov::Activity, fprov::Entity>> provUsedIterators = graph.getProvUsedIterators();
	for (auto it = provUsedIterators.first; it != provUsedIterators.second; ++it) {
		this->dump<fprov::Activity, fprov::Entity>(*it);
	}

	pair<fprov::EdgeIterator<fprov::Entity, fprov::Activity>, fprov::EdgeIterator<fprov::Entity, fprov::Activity>> provWasGeneratedBy = graph.getProvWasGeneratedByIterators();
	for (auto it = provWasGeneratedBy.first; it != provWasGeneratedBy.second; ++it) {
		this->dump<fprov::Entity, fprov::Activity>(*it);
	}

	for (unsigned activityId = 0; activityId < graph.getNumberOfActivities(); ++activityId) {
		fprov::Activity activity(activityId, graph.getDomain());
		this->dump<fprov::Activity, fprov::Agent>(activity, fprov::PROVO::wasAssociatedWith ,graph.getAgentsForActivity(activity));
	}

	unsigned firstSource = graph.getFirstSourceId();
	for (unsigned sourceEntityId = firstSource; sourceEntityId < firstSource + graph.getNumberOfSourceEntities(); ++sourceEntityId) {
		fprov::Entity entity(sourceEntityId, graph.getDomain());
		this->dump<fprov::Entity, fprov::Agent>(entity, fprov::PROVO::wasAttributedTo, graph.getAgentsForSource(entity));
	}
}

} /* namespace output */
} /* namespace fourbench */
