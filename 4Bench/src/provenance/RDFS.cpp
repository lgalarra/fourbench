/*
 * RDFS.cpp
 *
 *  Created on: Jul 25, 2017
 *      Author: galarraga
 */

#include "../include/provenance/RDFS.hpp"

namespace f = fourbench;

namespace fourbench {
namespace provenance {

const string RDFS::prefix = "http://www.w3.org/2000/01/rdf-schema#"
const string RDFS::label = f::concat({prefix, "label"});

RDFS::RDFS() {}

RDFS::~RDFS() {}

} /* namespace provenance */
} /* namespace fourbench */
