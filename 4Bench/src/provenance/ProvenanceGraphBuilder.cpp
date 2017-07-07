/*
 * ProvenanceGraphBuilder.cpp
 *
 *  Created on: Jul 3, 2017
 *      Author: galarraga
 */


#include <memory>
#include <vector>

#include "../include/conf/Conf.hpp"
#include "../include/parsing/ParsingStats.hpp"
#include "../include/provenance/ProvenanceGraphBuilder.hpp"
#include "../include/provenance/ProvenanceGraph.hpp"

namespace fp = fourbench::parsing;

namespace fourbench {
namespace provenance {

ProvenanceGraphBuilder ProvenanceGraphBuilder::instance;

ProvenanceGraphBuilder::ProvenanceGraphBuilder() {}

ProvenanceGraphBuilder::~ProvenanceGraphBuilder() {}

ProvenanceGraphBuilder& ProvenanceGraphBuilder::getInstance() {
	return instance;
}

vector<ProvenanceGraph*> ProvenanceGraphBuilder::buildProvenanceGraphs(const fc::Conf& conf,
		const fp::ParsingStats& stats) {
	vector<ProvenanceGraph*> result(conf.confs.size());
	for (auto itr = conf.confs.begin(); itr != conf.confs.end(); ++itr) {
		result.push_back(buildProvenanceGraph(*itr->second, stats));
	}

	return result;
}

ProvenanceGraph* ProvenanceGraphBuilder::buildProvenanceGraph(const fc::ConfValues& conf,
		const fp::ParsingStats& stats) {
	return new ProvenanceGraph(conf, stats);
}


} /* namespace provenance */
} /* namespace fourbench */
