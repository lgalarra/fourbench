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

	pair<fprov::EdgeIterator<fprov::Entity, fprov::Agent>, fprov::EdgeIterator<fprov::Entity, fprov::Agent>> provWasAttributedTo = graph.getProvWasAttributedToIterators();
	for (auto it = provWasAttributedTo.first; it != provWasAttributedTo.second; ++it) {
		this->dump<fprov::Entity, fprov::Agent>(*it);
	}
}

} /* namespace output */
} /* namespace fourbench */
