/*
 * IRIBuilder.cpp
 *
 *  Created on: Jul 11, 2017
 *      Author: galarraga
 */

#include <string>

#include "../include/provenance/IRIBuilder.hpp"
#include "../include/provenance/IRIType.hpp"

using namespace std;

namespace fourbench {
namespace provenance {

IRIBuilder IRIBuilder::instance;
string IRIBuilder::defaultDomain;

IRIBuilder::IRIBuilder() {

}

IRIBuilder::~IRIBuilder() {
	// TODO Auto-generated destructor stub
}

IRIBuilder& IRIBuilder::getInstance() {
	return instance;
}

string IRIBuilder::getDefaultDomain() {
	return defaultDomain;
}

string IRIBuilder::getIRI(IRIType type, unsigned id) const {
	return getIRI(defaultDomain, type, id);
}

string IRIBuilder::getIRI(const string& domain, IRIType type, unsigned id) const {
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


} /* namespace provenance */
} /* namespace fourbench */
