/*
 * ProvenanceGraphPopulator.cpp
 *
 *  Created on: Jul 11, 2017
 *      Author: galarraga
 */

#include <memory>
#include <map>

#include "../include/parsing/FileParser.hpp"
#include "../include/provenance/ProvenanceGraph.hpp"
#include "../include/provenance/ProvenanceGraphPopulator.hpp"

namespace fpa = fourbench::parsing;

namespace fourbench {
namespace provenance {

ProvenanceGraphPopulator::ProvenanceGraphPopulator() {}

void ProvenanceGraphPopulator::populate(fpa::FileParser& parser,
		map<string, shared_ptr<ProvenanceGraph>>& graphs) {
	fpa::Triple *triple = parser.next();
	while (triple != nullptr) {
		cout << *triple << ", " << parser.getCurrentFile() << ", " << parser.getLineNumber() << endl;
		delete triple;
		triple = parser.next();
	}
}

ProvenanceGraphPopulator::~ProvenanceGraphPopulator() {
	// TODO Auto-generated destructor stub
}

} /* namespace provenance */
} /* namespace fourbench */
