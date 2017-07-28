/*
 * ProvenanceSubgraph.cpp
 *
 *  Created on: Jul 3, 2017
 *      Author: galarraga
 */

#include <algorithm>
#include <memory>
#include <vector>
#include <set>

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

shared_ptr<vector<unsigned>> ProvenanceSubgraph::getDomain() const {
	set<unsigned> unique;
	for (auto it = matrix.begin(); it != matrix.end(); ++it) {
		unique.insert(it->first);
	}
	shared_ptr<vector<unsigned>> result = make_shared<vector<unsigned>>(unique.begin(), unique.end());
	return result;
}


} /* namespace provenance */
} /* namespace fourbench */
