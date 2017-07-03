/*
 * ProvenanceSubgraph.cpp
 *
 *  Created on: Jul 3, 2017
 *      Author: galarraga
 */

#include "../include/provenance/ProvenanceSubgraph.hpp"


using namespace std;

namespace fourbench {
namespace provenance {

ProvenanceSubgraph::ProvenanceSubgraph(const string& name): name(name) {

}

ProvenanceSubgraph::~ProvenanceSubgraph() {
	// TODO Auto-generated destructor stub
}

string ProvenanceSubgraph::getName() const {
	return name;
}

bool ProvenanceSubgraph::addEdge(unsigned source, unsigned target) {
	auto itr = matrix.equal_range(source);
	if (find(itr.first, itr.second, target)) {
		return true;
	}

	matrix.insert(make_pair(source, target));
	return false;
}


} /* namespace provenance */
} /* namespace fourbench */
