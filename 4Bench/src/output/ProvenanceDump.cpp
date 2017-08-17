/*
 * ProvenanceDump.cpp
 *
 *  Created on: Jul 19, 2017
 *      Author: galarraga
 */

#include <iostream>
#include <ostream>
#include <time.h>

#include "../include/datatypes/DataValue.hpp"
#include "../include/provenance/Vocabulary.hpp"
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
	if (stream.good())
		cerr << "Stream is good " << endl;
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

ostream& ProvenanceDump::formatBoolean(bool b) const {
	stream << "\"" << b << "\"^^";
	formatIRI(fprov::XSD::boolean);
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

ostream& ProvenanceDump::formatCountry(const string& str) const {
	cout << "IRIfying " << f::concat({fprov::Vocabulary::Country, f::wikify(str)}) << endl;
	formatIRI(f::concat({fprov::Vocabulary::Country, f::wikify(str)}));
	return stream;
}

string formatDayOrMonth(int dayOrMonth) {
	if (dayOrMonth < 10) {
		return f::concat({"0", to_string(dayOrMonth)});
	} else {
		return to_string(dayOrMonth);
	}
}

ostream& ProvenanceDump::formatDate(const fd::Date& date) const {
	stream << "\"" << formatDayOrMonth(date.day) << "-" << formatDayOrMonth(date.month) << "-" << date.year << "\"^^";
	formatIRI(fprov::XSD::date);
	return stream;
}

ostream& ProvenanceDump::formatDateTime(time_t tstamp) const {
	struct tm * tstruct = localtime(&tstamp);
	char buf[80];
	strftime(buf, sizeof(buf), "%Y-%m-%dT%XZ", tstruct);
	stream << "\"" << buf << "\"^^";
	formatIRI(fprov::XSD::dateTime);
	return stream;
}


ostream& ProvenanceDump::formatAgentType(fd::AgentTypeEnum type) const {
	string suffix;
	switch (type) {
	case fd::AgentTypeEnum::PERSON :
		suffix = "AgentType/Person/";
		break;
	case fd::AgentTypeEnum::ORGANIZATION :
		suffix = "AgentType/Organization/";
		break;
	case fd::AgentTypeEnum::SOFTWARE_AGENT :
		suffix = "AgentType/SoftwareAgent/";
		break;
	}

	this->formatIRI(f::concat({fprov::IRIBuilder::getDefaultPrefix(), suffix}));
	return stream;

}

ostream& ProvenanceDump::formatActivityType(fd::ActivityTypeEnum type) const {
	string suffix;
	switch (type) {
	case fd::ActivityTypeEnum::AGGREGATION :
		suffix = "ActivityType/Aggregation/";
		break;
	case fd::ActivityTypeEnum::EXTRACTION :
		suffix = "ActivityType/Extraction/";
		break;
	case fd::ActivityTypeEnum::JOIN :
		suffix = "ActivityType/Join";
		break;
	}

	this->formatIRI(f::concat({fprov::IRIBuilder::getDefaultPrefix(), suffix}));
	return stream;
}

ostream& ProvenanceDump::formatEntityType(fd::EntityTypeEnum type) const {
	string suffix;
	switch (type) {
	case fd::EntityTypeEnum::DATABASE :
		suffix = "EntityType/Database/";
		break;
	case fd::EntityTypeEnum::DATA_MAPPING :
		suffix = "EntityType/DataMapping";
		break;
	case fd::EntityTypeEnum::FILE :
		suffix = "EntityType/File";
		break;
	case fd::EntityTypeEnum::WEB_SERVICE :
		suffix = "EntityType/WebService";
		break;
	}

	this->formatIRI(f::concat({fprov::IRIBuilder::getDefaultPrefix(), suffix}));
	return stream;
}


ostream& ProvenanceDump::format(shared_ptr<fd::DataValue> value) const {
	fd::DataType& type = value->getType();
	string typeName = type.getName();
	if (typeName == "IRI") {
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
	} else if (typeName == "country") {
		return formatCountry(value->getAs<string>());
	} else if (typeName == "boolean") {
		return formatBoolean(value->getAs<bool>());
	} else if (typeName == "dateTime") {
		return formatDateTime(value->getAs<time_t>());
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
