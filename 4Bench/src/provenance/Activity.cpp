/*
 * Activity.cpp
 *
 *  Created on: Jul 7, 2017
 *      Author: galarraga
 */

#include <sstream>
#include <memory>

#include "../include/provenance/ProvenanceObject.hpp"
#include "../include/provenance/Activity.hpp"
#include "../include/provenance/IRIBuilder.hpp"
#include "../include/provenance/IRIType.hpp"
#include "../include/provenance/PROVO.hpp"
#include "../include/provenance/RDF.hpp"
#include "../include/provenance/RDFS.hpp"
#include "../include/utils/string.hpp"
#include "../include/datatypes/DataValueBuilder.hpp"
#include "../include/datatypes/DataValue.hpp"

using namespace std;

namespace fd = fourbench::datatypes;
namespace f = fourbench;

namespace fourbench {
namespace provenance {

Activity::Activity(unsigned id) : ProvenanceObject(id) {
}

Activity::Activity(unsigned id, const string& domain) : ProvenanceObject(id, domain) {
}

void Activity::initialize() {
	ProvenanceObject::initialize();
	fd::DataValueBuilder& dataBuilder = fd::DataValueBuilder::getInstance();
	attributes[RDF::type] = dataBuilder.get<fd::IRIValue>(getIRI());
	attributes[RDFS::label] = dataBuilder.get<fd::StringValue>(f::concat({"Activity ", to_string(id)}));
}

Activity::~Activity() {
}

string Activity::getIRI() const {
	shared_ptr<IRIBuilder> builder = IRIBuilder::getInstance(domain);
	return builder->getIRI(IRIType::ACTIVITY, id);
}

void Activity::populateWithAttributes(const fc::ConfValues& conf) {
	ProvenanceObject::populateWithAttributes(conf);
}

} /* namespace provenance */
} /* namespace fourbench */
