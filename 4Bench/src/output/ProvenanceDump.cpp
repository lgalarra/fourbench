/*
 * ProvenanceDump.cpp
 *
 *  Created on: Jul 19, 2017
 *      Author: galarraga
 */

#include <ostream>

#include "../include/output/ProvenanceDump.hpp"

using namespace std;

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

} /* namespace output */
} /* namespace fourbench */
