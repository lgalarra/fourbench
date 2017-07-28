/*
 * FOAF.cpp
 *
 *  Created on: Jul 28, 2017
 *      Author: galarraga
 */

#include "../include/provenance/FOAF.hpp"
#include "../include/utils/string.hpp"

#include <string>

namespace f = fourbench;

namespace fourbench {
namespace provenance {

const string FOAF::prefix = "http://xmlns.com/foaf/0.1/";
const string FOAF::Agent = f::concat({FOAF::prefix, "Agent"});
const string FOAF::Person = f::concat({FOAF::prefix, "Person"});
const string FOAF::Organization = f::concat({FOAF::prefix, "Organization"});

FOAF::FOAF() {}

FOAF::~FOAF() {}

} /* namespace provenance */
} /* namespace fourbench */
