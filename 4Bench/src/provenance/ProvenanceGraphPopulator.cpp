/*
 * ProvenanceGraphPopulator.cpp
 *
 *  Created on: Jul 11, 2017
 *      Author: galarraga
 */

#include <memory>
#include <map>

#include "../include/conf/Conf.hpp"
#include "../include/parsing/FileParser.hpp"
#include "../include/provenance/IRIBuilder.hpp"
#include "../include/provenance/ProvenanceAssignmentFactory.hpp"
#include "../include/provenance/ProvenanceGraph.hpp"
#include "../include/provenance/ProvenanceGraphPopulator.hpp"

namespace fpa = fourbench::parsing;
namespace fc = fourbench::conf;

using namespace std;

namespace fourbench {
namespace provenance {

ProvenanceGraphPopulator::ProvenanceGraphPopulator() {}

void ProvenanceGraphPopulator::populate(fpa::FileParser& parser,
		map<string, shared_ptr<ProvenanceGraph>>& graphs) {
	// Construct a map of provenance assignments
	map<string, shared_ptr<ProvenanceAssignment>> assignments;
	ProvenanceAssignmentFactory& assignFactory =
			ProvenanceAssignmentFactory::getInstance();
	fc::Conf& conf = fc::Conf::defaultConfig();
	IRIBuilder& builder = IRIBuilder::getInstance();

	for (auto itr = graphs.begin(); itr != graphs.end(); ++itr) {
		assignments[itr->first] =  assignFactory.getProvenanceAssignment(itr->second);
	}

	fpa::Triple *triple = parser.next();
	while (triple != nullptr) {
		cout << *triple << ", " << parser.getCurrentFile() << ", " << parser.getLineNumber() << endl;
		delete triple;
		string family = conf.getFamily(triple->getPredicate());
		auto assignItr = assignments.find(family);
		if (assignItr == assignments.end()) {
			cerr << "No family for triple " << *triple << endl;
		} else {
			// Generate a provenance identifier
			unsigned provenanceId = assignItr->second->nextProvenanceId();
			// TODO: Provide a different prefix for each provenance graph
			string provenanceIRI = builder.getIRI(IRIType::ENTITY, provenanceId);
			cout << *triple << ", " << provenanceIRI << endl;
		}

		triple = parser.next();
	}
}

ProvenanceGraphPopulator::~ProvenanceGraphPopulator() {
	// TODO Auto-generated destructor stub
}

} /* namespace provenance */
} /* namespace fourbench */
