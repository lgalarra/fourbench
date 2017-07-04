/*
 * ProvenanceGraphBuilder.cpp
 *
 *  Created on: Jul 3, 2017
 *      Author: galarraga
 */


#include <memory>
#include <vector>

#include "../include/conf/Conf.hpp"
#include "../include/provenance/ProvenanceGraphBuilder.hpp"
#include "../include/provenance/ProvenanceGraph.hpp"

namespace fourbench {
namespace provenance {

ProvenanceGraphBuilder ProvenanceGraphBuilder::instance;

ProvenanceGraphBuilder::ProvenanceGraphBuilder() {}

ProvenanceGraphBuilder::~ProvenanceGraphBuilder() {}

ProvenanceGraphBuilder& ProvenanceGraphBuilder::getInstance() {
	return instance;
}

vector<shared_ptr<ProvenanceGraph>> ProvenanceGraphBuilder::buildProvenanceGraphs(fc::Conf& conf) {
	vector<shared_ptr<ProvenanceGraph>> graphs;

	for (auto itr = conf.confs.begin(); itr != conf.confs.end(); ++itr) {
		shared_ptr<ProvenanceGraph> spt(new ProvenanceGraph(*(itr->second), 10));
		graphs.push_back(spt);
	}

	return graphs;
}


} /* namespace provenance */
} /* namespace fourbench */
