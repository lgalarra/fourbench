/*
 * ProvenanceGraphPopulator.cpp
 *
 *  Created on: Jul 11, 2017
 *      Author: galarraga
 */

#include <memory>
#include <map>

#include "../include/parsing/FileParser.hpp"
#include "../include/provenance/ProvenanceGraphPopulator.hpp"

namespace fpa = fourbench::parsing;

namespace fourbench {
namespace provenance {

ProvenanceGraphPopulator::ProvenanceGraphPopulator() {}

void ProvenanceGraphPopulator::populate(fpa::FileParser& parser,
		map<string, shared_ptr<ProvenanceGraph>>& graphs) {

}

ProvenanceGraphPopulator::~ProvenanceGraphPopulator() {
	// TODO Auto-generated destructor stub
}

} /* namespace provenance */
} /* namespace fourbench */
