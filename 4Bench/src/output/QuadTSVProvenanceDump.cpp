/*
 * QuadTSVProvenanceDump.cpp
 *
 *  Created on: Jul 19, 2017
 *      Author: galarraga
 */

#include <ostream>
#include <utility>

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
		//tuple<fprov::Activity, string, fprov::Entity> tuple = *it;

	}
}

} /* namespace output */
} /* namespace fourbench */
