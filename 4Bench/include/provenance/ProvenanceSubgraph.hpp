/*
 * ProvenanceSubgraph.hpp
 *
 *  Created on: Jul 3, 2017
 *      Author: galarraga
 */

#ifndef PROVENANCE_PROVENANCESUBGRAPH_HPP_
#define PROVENANCE_PROVENANCESUBGRAPH_HPP_

#include <string>
#include <map>
#include <memory>
#include <vector>

using namespace std;

namespace fourbench {
namespace provenance {

class ProvenanceSubgraph {

private:
	string name;
	multimap<unsigned, unsigned> matrix;

	ProvenanceSubgraph(const string& name);
public:

	string getName() const;

	bool addEdge(unsigned source, unsigned target);

	shared_ptr<vector<unsigned>> getDomain() const;

	virtual ~ProvenanceSubgraph();

	friend class ProvenanceGraph;
};

} /* namespace provenance */
} /* namespace fourbench */

#endif /* PROVENANCESUBGRAPH_HPP_ */
