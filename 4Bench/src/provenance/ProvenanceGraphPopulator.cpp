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

void ProvenanceGraphPopulator::populate(shared_ptr<fpa::FileParser> parser,
		shared_ptr<map<string, shared_ptr<ProvenanceGraph>>> graphs) {
	// Construct a map of provenance assignments
	map<string, shared_ptr<ProvenanceAssignment>> assignments;

	ProvenanceAssignmentFactory& assignFactory =
			ProvenanceAssignmentFactory::getInstance();
	fc::Conf& conf = fc::Conf::defaultConfig();

	cout << "Initializing the assignments" << endl;
	for (auto itr = graphs->begin(); itr != graphs->end(); ++itr) {
		cout << "Assignment for " << itr->first << endl;
		assignments[itr->first] = assignFactory.getProvenanceAssignment(itr->second);
	}

	fpa::Triple *triple = parser->next();
	if (triple == nullptr)
		cerr << "First triple is null" << endl;
	while (triple != nullptr) {
		string family = conf.getFamily(triple->getPredicate());
		auto assignItr = assignments.find(family);
		if (assignItr == assignments.end()) {
			cerr << "No family for triple " << *triple << endl;
		} else {
			// Generate a provenance identifier
			cout << "======================" << endl;
			shared_ptr<ProvenanceGraph> graphPtr = assignItr->second->getGraph();
			Entity provenanceEntity(assignItr->second->nextProvenanceId(), graphPtr->getDomain());
			cout << "Object created" << endl;
			output->dump(*triple, provenanceEntity);
			cout << *triple << ", " << provenanceEntity.getIRI() << endl;
			cout << "======================" << endl;
		}
		delete triple;
		triple = parser->next();
	}
	output->flush();

	for (auto itr = graphs->begin(); itr != graphs->end(); ++itr) {
		output->dump(itr->second);
		output->flush();
	}
}

ProvenanceGraphPopulator::~ProvenanceGraphPopulator() {
	// TODO Auto-generated destructor stub
}

} /* namespace provenance */
} /* namespace fourbench */
