/*
 * ProvenanceSubgraph.cpp
 *
 *  Created on: Jul 3, 2017
 *      Author: galarraga
 */

#include <algorithm>

#include "../include/provenance/ProvenanceSubgraph.hpp"


using namespace std;

namespace fourbench {
namespace provenance {

ProvenanceSubgraph::ProvenanceSubgraph(const string& name): name(name) {}

ProvenanceSubgraph::~ProvenanceSubgraph() {}

string ProvenanceSubgraph::getName() const {
	return name;
}

bool ProvenanceSubgraph::addEdge(unsigned source, unsigned target) {
	auto itr = matrix.equal_range(source);
	auto itrNav = itr.first;
	while (itrNav != itr.second) {
		if (itrNav->second == target) {
			return true;
		}
		++itrNav;
	}

	matrix.insert(make_pair(source, target));
	return false;
}


} /* namespace provenance */
} /* namespace fourbench */
