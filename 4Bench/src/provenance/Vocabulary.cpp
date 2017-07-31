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

const string Vocabulary::sourceType = f::concat({IRIBuilder::getDefaultPrefix(), "property/sourceType/"});
const string Vocabulary::agentType = f::concat({IRIBuilder::getDefaultPrefix(), "property/agentType/"});
const string Vocabulary::SoftwareAgent = f::concat({IRIBuilder::getDefaultPrefix(), "type/SoftwareAgent/"});
const string Vocabulary::id = f::concat({IRIBuilder::getDefaultPrefix(), "property/id/"});

Vocabulary::Vocabulary() {
	// TODO Auto-generated constructor stub

}

Vocabulary::~Vocabulary() {
	// TODO Auto-generated destructor stub
}

} /* namespace provenance */
} /* namespace fourbench */
