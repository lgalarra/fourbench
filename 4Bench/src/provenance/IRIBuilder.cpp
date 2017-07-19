/*
 * IRIBuilder.cpp
 *
 *  Created on: Jul 11, 2017
 *      Author: galarraga
 */

#include <map>
#include <memory>
#include <string>
#include <sstream>

#include "../include/provenance/IRIBuilder.hpp"
#include "../include/provenance/IRIType.hpp"

using namespace std;

namespace fourbench {
namespace provenance {

const string IRIBuilder::defaultDomain = "http://fourbench.org/provenance/";
map<string, shared_ptr<IRIBuilder>> IRIBuilder::instances;

IRIBuilder::IRIBuilder(const string& prefix) : domain(prefix) {}

IRIBuilder::~IRIBuilder() {
}

shared_ptr<IRIBuilder> IRIBuilder::getInstance(const string& prefix) {
	if (instances.find(prefix) == instances.end()) {
		instances[prefix] = shared_ptr<IRIBuilder>(new IRIBuilder(prefix));
	}

	return instances[prefix];
}

string IRIBuilder::getDefaultDomain() {
	return defaultDomain;
}

string IRIBuilder::getIRI(IRIType type, unsigned id) const {
	stringstream sstrm;
	switch (type) {
	case ENTITY:
		sstrm << domain << "entity/" << id;
		break;
	case ACTIVITY:
		sstrm << domain << "activity/" << id;
		break;
	case AGENT:
		sstrm << domain << "agent/" << id;
		break;
	}

	return sstrm.str();
}

string IRIBuilder::getDomain() const {
	return domain;
}

} /* namespace provenance */
} /* namespace fourbench */
