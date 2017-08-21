/*
 * Activity.cpp
 *
 *  Created on: Jul 7, 2017
 *      Author: galarraga
 */

#include <sstream>
#include <memory>
#include <ctime>

#include "../include/provenance/ProvenanceObject.hpp"
#include "../include/provenance/Activity.hpp"
#include "../include/provenance/IRIBuilder.hpp"
#include "../include/provenance/IRIType.hpp"
#include "../include/provenance/PROVO.hpp"
#include "../include/provenance/RDF.hpp"
#include "../include/provenance/RDFS.hpp"
#include "../include/utils/string.hpp"
#include "../include/utils/integer.hpp"
#include "../include/datatypes/DataValueBuilder.hpp"
#include "../include/datatypes/DataValue.hpp"

using namespace std;

namespace fd = fourbench::datatypes;
namespace f = fourbench;

namespace fourbench {
namespace provenance {

Activity::Activity(unsigned id) : ProvenanceObject(id, IRIBuilder::getDefaultDomain(), 0) {
	initialize();
}

Activity::Activity(unsigned id, const string& domain) : ProvenanceObject(id, domain, 0) {
	initialize();
}

Activity::Activity(unsigned id, const string& domain, unsigned maxNumberOfAttributes) : ProvenanceObject(id, domain, maxNumberOfAttributes) {
	initialize();
}

void Activity::initialize() {
	ProvenanceObject::initialize();
	time_t now = time(nullptr);
	time_t before = now - (time_t) f::urand(1, 3600 * 48); // random timestamp between now and 48 hours ago
	fd::DataValueBuilder& dataBuilder = fd::DataValueBuilder::getInstance();
	attributes[RDF::type] = dataBuilder.get<fd::IRIValue>(PROVO::Activity);
	attributes[RDFS::label] = dataBuilder.get<fd::StringValue>(f::concat({"Activity ", to_string(id)}));
	attributes[PROVO::startedAtTime] = dataBuilder.get<fd::DateTimeValue>(before);
	attributes[PROVO::endedAtTime] = dataBuilder.get<fd::DateTimeValue>(now);
}

Activity::~Activity() {
}

string Activity::getIRI() const {
	shared_ptr<IRIBuilder> builder = IRIBuilder::getInstance(domain);
	return builder->getIRI(IRIType::ACTIVITY, id);
}

} /* namespace provenance */
} /* namespace fourbench */
