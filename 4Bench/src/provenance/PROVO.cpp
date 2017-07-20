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

PROVO::PROVO() {}

PROVO::~PROVO() {}

} /* namespace output */
} /* namespace fourbench */
