/*
 * ProvenanceDump.cpp
 *
 *  Created on: Jul 19, 2017
 *      Author: galarraga
 */

#include <ostream>

#include "../include/provenance/ProvenanceObject.hpp"
#include "../include/output/ProvenanceDump.hpp"

using namespace std;

namespace fprov = fourbench::provenance;

namespace fourbench {
namespace output {

ProvenanceDump::ProvenanceDump(ostream& strm) : stream(strm) {
}

ProvenanceDump::~ProvenanceDump() {
}

void ProvenanceDump::flush() {
	stream.flush();
}

ostream& ProvenanceDump::formatIRI(const string& iri) const {
	stream << "<" << iri << ">";
	return stream;
}

virtual ostream& ProvenanceDump::format(shared_ptr<fd::DataValue> value) const {
	fd::DataType& type = value->getType();
	string typeName = type.getName();
	if (typeName == "IRI" || typeName == "Country" || typeName == "AgentType") {
		return formatIRI(value->getAs<string>());
	}

	return stream;
}


void ProvenanceDump::dump(const fprov::ProvenanceObject& obj) const {
	pair<map<string, shared_ptr<fd::DataValue>>::const_iterator, map<string, shared_ptr<fd::DataValue>>::const_iterator> itsPair =
			obj.getAttributeIterators();
	for (auto it = itsPair.first; it != itsPair.second; ++it) {
		dump(obj, it->first, it->second);
	}
}

void ProvenanceDump::dump(const fprov::ProvenanceObject& obj, string attributeName, shared_ptr<fd::DataValue> attributeValue) const {}


} /* namespace output */
} /* namespace fourbench */
