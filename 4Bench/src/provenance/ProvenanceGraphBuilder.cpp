/*
 * ProvenanceGraphBuilder.cpp
 *
 *  Created on: Jul 3, 2017
 *      Author: galarraga
 */


#include <memory>
#include <vector>

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
		graphs.push_back(make_shared<ProvenanceGraph>(new ProvenanceGraph(*itr)));
	}

	return graphs;
}


} /* namespace provenance */
} /* namespace fourbench */
