/*
 * ProvenanceGraphFactory.cpp
 *
 *  Created on: Jul 3, 2017
 *      Author: galarraga
 */


#include <memory>
#include <map>

#include "../include/conf/Conf.hpp"
#include "../include/parsing/ParsingStats.hpp"
#include "../include/provenance/ProvenanceGraph.hpp"
#include "../include/provenance/ProvenanceGraphFactory.hpp"

namespace fp = fourbench::parsing;
using namespace std;

namespace fourbench {
namespace provenance {

ProvenanceGraphFactory ProvenanceGraphFactory::instance;

ProvenanceGraphFactory::ProvenanceGraphFactory() {}

ProvenanceGraphFactory::~ProvenanceGraphFactory() {}

ProvenanceGraphFactory& ProvenanceGraphFactory::getInstance() {
	return instance;
}

shared_ptr<map<string, shared_ptr<ProvenanceGraph>>> ProvenanceGraphFactory::buildProvenanceGraphs(const fc::Conf& conf,
		const shared_ptr<map<string, fp::ParsingStats>> stats) {
	shared_ptr<map<string, shared_ptr<ProvenanceGraph>>> resultPtr(new map<string, shared_ptr<ProvenanceGraph>>);

	for (auto itr = conf.confs.begin(); itr != conf.confs.end(); ++itr) {
		fp::ParsingStats stat; // bind stat to a value
		resultPtr.get()->insert(pair<string, shared_ptr<ProvenanceGraph>>(itr->first, buildProvenanceGraph(*itr->second, stat)));
	}

	return resultPtr;
}

shared_ptr<ProvenanceGraph> ProvenanceGraphFactory::buildProvenanceGraph(const fc::ConfValues& conf,
		const fp::ParsingStats& stats) {
	return shared_ptr<ProvenanceGraph>(new ProvenanceGraph(conf, stats));
}


} /* namespace provenance */
} /* namespace fourbench */
