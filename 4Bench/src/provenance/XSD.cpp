/*
 * XSD.cpp
 *
 *  Created on: Jul 29, 2017
 *      Author: galarraga
 */

#include "../include/utils/string.hpp"
#include "../include/provenance/XSD.hpp"

namespace f = fourbench;

namespace fourbench {
namespace provenance {

const string XSD::prefix = "http://www.w3.org/2001/XMLSchema#";
const string XSD::integer = f::concat({XSD::prefix, "integer"});
const string XSD::decimal = f::concat({XSD::prefix, "decimal"});
const string XSD::str = f::concat({XSD::prefix, "string"});
const string XSD::date = f::concat({XSD::prefix, "date"});
const string XSD::dateTime = f::concat({XSD::prefix, "dateTime"});
const string XSD::boolean = f::concat({XSD::prefix, "boolean"});


XSD::XSD() {}

XSD::~XSD() {}

} /* namespace provenance */
} /* namespace fourbench */
