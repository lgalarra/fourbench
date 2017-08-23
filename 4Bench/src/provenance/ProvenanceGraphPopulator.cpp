/*
 * ProvenanceGraphPopulator.cpp
 *
 *  Created on: Jul 11, 2017
 *      Author: galarraga
 */

#include <memory>
#include <map>
#include <chrono>
#include <utility>

#include "../include/utils/time.hpp"
#include "../include/utils/string.hpp"
#include "../include/conf/Conf.hpp"
#include "../include/parsing/FileParser.hpp"
#include "../include/provenance/IRIBuilder.hpp"
#include "../include/output/ProvenanceDump.hpp"
#include "../include/provenance/ProvenanceAssignmentFactory.hpp"
#include "../include/provenance/ProvenanceGraph.hpp"
#include "../include/provenance/ProvenanceGraphPopulator.hpp"
#include "../include/provenance/Entity.hpp"
#include "../include/provenance/PopulateStats.hpp"

namespace fpa = fourbench::parsing;
namespace fc = fourbench::conf;
namespace f = fourbench;

using namespace std;

namespace fourbench {
namespace provenance {

ProvenanceGraphPopulator::ProvenanceGraphPopulator(shared_ptr<fo::ProvenanceDump> out) : output(out) {}

shared_ptr<map<string, shared_ptr<PopulateStats>>> ProvenanceGraphPopulator::populate(shared_ptr<fpa::FileParser> parser,
		shared_ptr<map<string, shared_ptr<ProvenanceGraph>>> graphs) {
	// Construct a map of provenance assignments
	shared_ptr<map<string, shared_ptr<PopulateStats>>> popStats =
			make_shared<map<string, shared_ptr<PopulateStats>>>();
	map<string, shared_ptr<ProvenanceAssignment>> assignments;

	ProvenanceAssignmentFactory& assignFactory =
			ProvenanceAssignmentFactory::getInstance();
	fc::Conf& conf = fc::Conf::defaultConfig();

	for (auto itr = graphs->begin(); itr != graphs->end(); ++itr) {
		assignments[itr->first] = assignFactory.getProvenanceAssignment(itr->second);
		(*popStats)[itr->first] = make_shared<PopulateStats>();
	}

	chrono::steady_clock::time_point tstart = f::time();
	fpa::Triple *triple = parser->next();
	while (triple != nullptr) {
		string family = conf.getFamily(triple->getPredicate());
		auto assignItr = assignments.find(family);
		if (assignItr == assignments.end()) {
			cerr << "No family for triple " << *triple << endl;
		} else {
			// Generate a provenance identifier
			shared_ptr<ProvenanceGraph> graphPtr = assignItr->second->getGraph();
			Entity provenanceEntity(assignItr->second->nextProvenanceId(), graphPtr->getDomain());
			output->dump(*triple, provenanceEntity);
#ifdef DEBUG
			cout << *triple << " assigned provenance " << provenanceEntity.getIRI() << endl;
#endif
		}
		delete triple;
		triple = parser->next();
	}
	output->flush();
#ifdef DEBUG
	cout << "Assignment of triples to provenance identifiers took " << f::formatElapsedTime(tstart, f::time()) << endl;
#endif

	cout << "Outputting the provenance graphs" << endl;
	for (auto itr = graphs->begin(); itr != graphs->end(); ++itr) {
		cout << "Outputting provenance graph for family \"" << itr->first << "\"" << endl;
		tstart = f::time();
		output->dump(itr->second);
		output->flush();
#ifdef DEBUG
	cout << "Output of graph for family \"" << itr->first << "\" took " << f::formatElapsedTime(tstart, f::time()) << endl;
#endif
		(*popStats)[itr->first]->familyName = itr->first;
		(*popStats)[itr->first]->numberOfProvenanceIdentifiers = assignments[itr->first]->getNumberOfAssignedProvenanceIds();
	}

	return popStats;
}

ProvenanceGraphPopulator::~ProvenanceGraphPopulator() {
	// TODO Auto-generated destructor stub
}

} /* namespace provenance */
} /* namespace fourbench */
