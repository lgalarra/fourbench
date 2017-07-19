/*
 * ProvenanceGraphPopulator.cpp
 *
 *  Created on: Jul 11, 2017
 *      Author: galarraga
 */

#include <memory>
#include <map>

#include "../include/utils/string.hpp"
#include "../include/conf/Conf.hpp"
#include "../include/parsing/FileParser.hpp"
#include "../include/provenance/IRIBuilder.hpp"
#include "../include/output/ProvenanceDump.hpp"
#include "../include/provenance/ProvenanceAssignmentFactory.hpp"
#include "../include/provenance/ProvenanceGraph.hpp"
#include "../include/provenance/ProvenanceGraphPopulator.hpp"
#include "../include/provenance/Entity.hpp"

namespace fpa = fourbench::parsing;
namespace fc = fourbench::conf;
namespace f = fourbench;

using namespace std;

namespace fourbench {
namespace provenance {

ProvenanceGraphPopulator::ProvenanceGraphPopulator(shared_ptr<fo::ProvenanceDump> out) : output(out) {}

void ProvenanceGraphPopulator::populate(fpa::FileParser& parser,
		map<string, shared_ptr<ProvenanceGraph>>& graphs) {
	// Construct a map of provenance assignments
	map<string, shared_ptr<ProvenanceAssignment>> assignments;
	// IRIBuilders
	map<string, shared_ptr<IRIBuilder>> iriBuilders;

	ProvenanceAssignmentFactory& assignFactory =
			ProvenanceAssignmentFactory::getInstance();
	fc::Conf& conf = fc::Conf::defaultConfig();

	for (auto itr = graphs.begin(); itr != graphs.end(); ++itr) {
		assignments[itr->first] = assignFactory.getProvenanceAssignment(itr->second);
		iriBuilders[itr->first] = IRIBuilder::getInstance(
				f::concat({IRIBuilder::getDefaultDomain(), itr->first, "/"}));
	}

	fpa::Triple *triple = parser.next();
	while (triple != nullptr) {
		string family = conf.getFamily(triple->getPredicate());
		auto assignItr = assignments.find(family);
		if (assignItr == assignments.end()) {
			cerr << "No family for triple " << *triple << endl;
		} else {
			// Generate a provenance identifier
			cout << "======================" << endl;
			shared_ptr<IRIBuilder> iriBuilder = iriBuilders[family];
			Entity provenanceEntity(assignItr->second->nextProvenanceId(), iriBuilder->getDomain());
			output->dump(*triple, provenanceEntity);
			cout << *triple << ", " << provenanceEntity.getIRI() << endl;
			cout << "======================" << endl;
		}
		delete triple;
		triple = parser.next();
	}

	for (auto itr = graphs.begin(); itr != graphs.end(); ++itr) {
		output->dump(*itr->second);
	}
}

ProvenanceGraphPopulator::~ProvenanceGraphPopulator() {
	// TODO Auto-generated destructor stub
}

} /* namespace provenance */
} /* namespace fourbench */
