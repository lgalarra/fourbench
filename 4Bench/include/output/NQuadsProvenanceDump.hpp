/*
 * NQuadsProvenanceDump.hpp
 *
 *  Created on: Jul 19, 2017
 *      Author: galarraga
 */

#ifndef OUTPUT_NQUADSPROVENANCEDUMP_HPP_
#define OUTPUT_NQUADSPROVENANCEDUMP_HPP_

#include <vector>

#include "../include/output/ProvenanceDump.hpp"
#include "../include/provenance/ProvenanceGraph.hpp"

namespace fpar = fourbench::parsing;
namespace fprov = fourbench::provenance;

using namespace std;

namespace fourbench {
namespace output {

class NQuadsProvenanceDump: public ProvenanceDump {
private:

	template<class Domain, class Range> inline void dump(const tuple<shared_ptr<Domain>, string, shared_ptr<Range>>& tuple) {
		this->formatIRI(get<0>(tuple)->getIRI());
		stream << " ";
		this->formatIRI(get<1>(tuple));
		stream << " ";
		this->formatIRI(get<2>(tuple)->getIRI());
		stream << " ";
		this->formatIRI(fprov::ProvenanceGraph::getDefaultProvenanceGraphIRI());
		stream << " .";
		stream << endl;
		++provenanceTriplesDumped;
	}

	template<class Domain, class Range> void dump(const Domain& domain, const string& property, vector<shared_ptr<Range>> ranges) {
		for (auto itr = ranges.begin(); itr != ranges.end(); ++itr) {
			this->formatIRI(domain.getIRI());
			stream << " ";
			this->formatIRI(property);
			stream << " ";
			this->formatIRI((*itr)->getIRI());
			stream << " ";
			this->formatIRI(fprov::ProvenanceGraph::getDefaultProvenanceGraphIRI());
			stream << " .";
			stream << endl;
			++provenanceTriplesDumped;
		}
	}

protected:
	virtual void dump(const fprov::ProvenanceObject& obj, string attributeName, shared_ptr<fd::DataValue> attributeValue);
	virtual void dump(const fprov::ProvenanceObject& obj);

public:
	NQuadsProvenanceDump(ostream& strm);
	virtual void dump(const fpar::Triple& triple, const fprov::Entity& provEntity);
	virtual void dump(shared_ptr<fprov::ProvenanceGraph> graph);
	virtual ~NQuadsProvenanceDump();
};

} /* namespace output */
} /* namespace fourbench */

#endif /* NQuadsProvenanceDump_HPP_ */
