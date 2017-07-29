/*
 * ProvenanceDump.cpp
 *
 *  Created on: Jul 19, 2017
 *      Author: galarraga
 */

#include <ostream>

#include "../include/datatypes/DataValue.hpp"
#include "../include/provenance/XSD.hpp"
#include "../include/provenance/ProvenanceObject.hpp"
#include "../include/output/ProvenanceDump.hpp"

using namespace std;

namespace fprov = fourbench::provenance;
namespace fd = fourbench::datatypes;

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

ostream& ProvenanceDump::formatInteger(int integer) const {
	stream << "\"" << integer << "\"^^";
	formatIRI(fprov::XSD::integer);
	return stream;
}

ostream& ProvenanceDump::formatFloat(float decimal) const {
	stream << "\"" << decimal << "\"^^";
	formatIRI(fprov::XSD::decimal);
	return stream;
}

ostream& ProvenanceDump::formatString(const string& str) const {
	stream << "\"" << str << "\"^^";
	formatIRI(fprov::XSD::str);
	return stream;
}

ostream& ProvenanceDump::formatDate(const fd::Date& date) const {
	stream << "\"" << date.day << "-" << (date.month < 10 ? f::concat({"0", to_string(date.month)}) : to_string(date.month)) << "-" << date.year << "\"^^";
	formatIRI(fprov::XSD::date);
	return stream;
}

ostream& ProvenanceDump::formatAgentType(fd::AgentTypeEnum type) const {

	return stream;
}

ostream& ProvenanceDump::formatActivityType(fd::ActivityTypeEnum type) const {
	return stream;
}

ostream& ProvenanceDump::formatEntityType(fd::EntityTypeEnum type) const {
	return stream;
}

ostream& ProvenanceDump::format(shared_ptr<fd::DataValue> value) const {
	fd::DataType& type = value->getType();
	string typeName = type.getName();
	cout << "ProvenanceDump::format " << typeName << endl;
	if (typeName == "IRI" || typeName == "country") {
		return formatIRI(value->getAs<string>());
	} else if (typeName == "integer") {
		return formatInteger(value->getAs<int>());
	} else if (typeName == "ratio") {
		return formatFloat(value->getAs<float>());
	} else if (typeName == "date") {
		return formatDate(value->getAs<fd::Date>());
	} else if (typeName == "string") {
		return formatString(value->getAs<string>());
	} else if (typeName == "agentType") {
		return formatAgentType(value->getAs<fd::AgentTypeEnum>());
	} else if (typeName == "entityType") {
		return formatEntityType(value->getAs<fd::EntityTypeEnum>());
	} else if (typeName == "activityType") {
		return formatActivityType(value->getAs<fd::ActivityTypeEnum>());
	}

	return stream;
}


void ProvenanceDump::dump(const fprov::ProvenanceObject& obj) const {
	pair<map<string, shared_ptr<fd::DataValue>>::const_iterator, map<string, shared_ptr<fd::DataValue>>::const_iterator> itsPair =
			obj.getAttributeIterators();
	for (auto it = itsPair.first; it != itsPair.second; ++it) {
		this->dump(obj, it->first, it->second);
	}
}



} /* namespace output */
} /* namespace fourbench */
