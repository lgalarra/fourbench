/*
 * Vocabulary.cpp
 *
 *  Created on: Jul 28, 2017
 *      Author: galarraga
 */

#include <string>

#include "../include/utils/string.hpp"
#include "../include/provenance/IRIBuilder.hpp"
#include "../include/provenance/Vocabulary.hpp"

using namespace std;
namespace f = fourbench;

namespace fourbench {
namespace provenance {

const string Vocabulary::property = f::concat({IRIBuilder::getDefaultPrefix(), "property/"});
const string Vocabulary::sourceType = f::concat({Vocabulary::property, "sourceType/"});
const string Vocabulary::agentType = f::concat({Vocabulary::property, "agentType/"});
const string Vocabulary::SoftwareAgent = f::concat({IRIBuilder::getDefaultPrefix(), "type/SoftwareAgent/"});
const string Vocabulary::Country = f::concat({IRIBuilder::getDefaultPrefix(), "type/Country/"});
const string Vocabulary::id = f::concat({Vocabulary::property, "id/"});

Vocabulary::Vocabulary() {}

Vocabulary::~Vocabulary() {}

} /* namespace provenance */
} /* namespace fourbench */
