/*
 * PROVO.cpp
 *
 *  Created on: Jul 20, 2017
 *      Author: galarraga
 */

#include "../include/provenance/PROVO.hpp"
#include "../include/utils/string.hpp"

namespace f = fourbench;

namespace fourbench {
namespace provenance {

const string PROVO::prefix = "http://www.w3.org/ns/prov#";
const string PROVO::wasAttributedTo = f::concat({PROVO::prefix, "wasAttributedTo"});
const string PROVO::wasAssociatedWith = f::concat({PROVO::prefix, "wasAssociatedWith"});
const string PROVO::used = f::concat({PROVO::prefix, "used"});
const string PROVO::wasGeneratedBy = f::concat({PROVO::prefix, "wasGeneratedBy"});
const string PROVO::startedAtTime = f::concat({PROVO::prefix, "startedAtTime"});
const string PROVO::endedAtTime = f::concat({PROVO::prefix, "endedAtTime"});
const string PROVO::Activity = f::concat({PROVO::prefix, "Activity"});
const string PROVO::Agent = f::concat({PROVO::prefix, "Agent"});
const string PROVO::Entity = f::concat({PROVO::prefix, "Entity"});
const string PROVO::Person = f::concat({PROVO::prefix, "Person"});
const string PROVO::Organization = f::concat({PROVO::prefix, "Organization"});
const string PROVO::SoftwareAgent = f::concat({PROVO::prefix, "SoftwareAgent"});
const string PROVO::Collection = f::concat({PROVO::prefix, "Collection"});
const string PROVO::Bundle = f::concat({PROVO::prefix, "Bundle"});
const string PROVO::Plan = f::concat({PROVO::prefix, "Plan"});

PROVO::PROVO() {}

PROVO::~PROVO() {}

} /* namespace output */
} /* namespace fourbench */
