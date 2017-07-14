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
#include "../include/provenance/ProvenanceAssignmentFactory.hpp"
#include "../include/provenance/ProvenanceGraph.hpp"
#include "../include/provenance/ProvenanceGraphPopulator.hpp"

namespace fpa = fourbench::parsing;
namespace fc = fourbench::conf;
namespace f = fourbench;

using namespace std;

namespace fourbench {
namespace provenance {

ProvenanceGraphPopulator::ProvenanceGraphPopulator() {}

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
			unsigned provenanceId = assignItr->second->nextProvenanceId();
			shared_ptr<IRIBuilder> iriBuilder = iriBuilders[family];
			string provenanceIRI = iriBuilder->getIRI(IRIType::ENTITY, provenanceId);
			cout << *triple << ", " << provenanceIRI << endl;
			cout << "======================" << endl;
		}
		delete triple;
		triple = parser.next();
	}
}

ProvenanceGraphPopulator::~ProvenanceGraphPopulator() {
	// TODO Auto-generated destructor stub
}

} /* namespace provenance */
} /* namespace fourbench */
