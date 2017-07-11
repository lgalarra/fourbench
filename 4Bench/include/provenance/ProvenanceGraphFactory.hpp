/*
 * ProvenanceGraphBuilder.hpp
 *
 * It takes a configuration object and constructs a set of provenance graphs
 *
 *  Created on: Jul 3, 2017
 *      Author: galarraga
 */

#ifndef PROVENANCE_PROVENANCEGRAPHBUILDER_HPP_
#define PROVENANCE_PROVENANCEGRAPHBUILDER_HPP_

#include <memory>
#include <map>

#include "../include/conf/Conf.hpp"
#include "ProvenanceGraph.hpp"

namespace fc = fourbench::conf;

using namespace std;

namespace fourbench {
namespace provenance {

class ProvenanceGraphFactory {
private:
	static ProvenanceGraphFactory instance;
	ProvenanceGraphFactory();

public:
	static ProvenanceGraphFactory& getInstance();
	virtual ~ProvenanceGraphFactory();
	shared_ptr<ProvenanceGraph> buildProvenanceGraph(const fc::ConfValues& conf,
			const fp::ParsingStats& stats);
	shared_ptr<map<string, shared_ptr<ProvenanceGraph>>> buildProvenanceGraphs(const fc::Conf& conf,
			const shared_ptr<map<string, fp::ParsingStats>> stats);
};

} /* namespace provenance */
} /* namespace fourbench */

#endif /* PROVENANCE_PROVENANCEGRAPHBUILDER_HPP_ */
