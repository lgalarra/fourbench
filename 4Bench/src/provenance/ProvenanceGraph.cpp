/*
 * ProvenanceGraph.cpp
 *
 *  Created on: Jul 3, 2017
 *      Author: galarraga
 */

#include "../include/conf/Conf.hpp"
#include "../include/conf/AssignmentDistribution.hpp"
#include "../include/provenance/ProvenanceGraph.hpp"

namespace fc = fourbench::conf;

namespace fourbench {
namespace provenance {

ProvenanceGraph::ProvenanceGraph(const fc::ConfValues& values, unsigned nTriples) {
	computeNumberOfLeafEntities(values, nTriples);

}

void ProvenanceGraph::computeNumberOfLeafEntities(const fc::ConfValues& values, unsigned nTriples) {
	const fc::Conf& conf = fc::Conf::defaultConfig();
	switch (values.distribution) {
	case fc::AssignmentDistribution::UNIFORM :
		leafEntities = 1;
		break;
	case fc::AssignmentDistribution::POWER_LAW :
		leafEntities = 2;
		break;
	case fc::AssignmentDistribution::GEOMETRIC :
		leafEntities = 3;
		break;
	}
}

ProvenanceGraph::~ProvenanceGraph() {
	// TODO Auto-generated destructor stub
}


} /* namespace provenance */
} /* namespace fourbench */
