/*
 * ProvenanceSubgraph.hpp
 *
 *  Created on: Jul 3, 2017
 *      Author: galarraga
 */

#ifndef PROVENANCESUBGRAPH_HPP_
#define PROVENANCESUBGRAPH_HPP_

#include <string>

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

	virtual ~ProvenanceSubgraph();

	friend class ProvenanceGraph;
};

} /* namespace provenance */
} /* namespace fourbench */

#endif /* PROVENANCESUBGRAPH_HPP_ */
