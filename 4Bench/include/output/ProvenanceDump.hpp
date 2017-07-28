/*
 * ProvenanceDump.hpp
 *
 *  Created on: Jul 19, 2017
 *      Author: galarraga
 */

#ifndef OUTPUT_PROVENANCEDUMP_HPP_
#define OUTPUT_PROVENANCEDUMP_HPP_

#include <ostream>
#include <vector>

#include "../include/parsing/Triple.hpp"
#include "../include/provenance/ProvenanceObject.hpp"
#include "../include/provenance/Entity.hpp"
#include "../include/provenance/ProvenanceGraph.hpp"

namespace fpar = fourbench::parsing;
namespace fprov = fourbench::provenance;
namespace fd = fourbench::datatypes;

using namespace std;

namespace fourbench {
namespace output {

class ProvenanceDump {
protected:
	ostream &stream;

	ProvenanceDump(ostream& strm);
	virtual ostream& formatIRI(const string& iri) const;
	virtual ostream& format(shared_ptr<fd::DataValue> value) const;
	virtual void dump(const fprov::ProvenanceObject& obj, string attributeName, shared_ptr<fd::DataValue> attributeValue) const;
	virtual void dump(const fprov::ProvenanceObject& obj) const;
public:
	virtual void dump(const fpar::Triple& triple, const fprov::Entity& provEntity) const = 0;
	// TODO: Change the signature to receive a constant graph
	virtual void dump(fprov::ProvenanceGraph& graph) const = 0;
	void flush();
	virtual ~ProvenanceDump();

	friend class ProvenanceDumpFactory;
};

} /* namespace output */
} /* namespace fourbench */

#endif /* OUTPUT_PROVENANCEDUMP_HPP_ */
