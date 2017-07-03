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
#include <vector>

#include "../include/conf/Conf.hpp"
#include "../include/provenance/ProvenanceGraph.hpp"

namespace fc = fourbench::conf;

using namespace std;

namespace fourbench {
namespace provenance {

class ProvenanceGraphBuilder {
private:
	static ProvenanceGraphBuilder instance;
	ProvenanceGraphBuilder();

public:
	static ProvenanceGraphBuilder& getInstance();
	virtual ~ProvenanceGraphBuilder();
	vector<shared_ptr<ProvenanceGraph>> buildProvenanceGraphs(fc::Conf& conf);
};

} /* namespace provenance */
} /* namespace fourbench */

#endif /* PROVENANCE_PROVENANCEGRAPHBUILDER_HPP_ */
