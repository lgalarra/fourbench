/*
 * ProvenanceGraphTest.cpp
 * Simple test for the ProvenanceGraph class
 *  Created on: Jul 7, 2017
 *      Author: galarraga
 */

#define BOOST_TEST_MODULE fourbench_test_tsvparser



#include <fstream>
#include <iostream>
#include <boost/test/included/unit_test.hpp>

#include "../include/conf/Conf.hpp"
#include "../include/provenance/ProvenanceGraph.hpp"
#include "../include/provenance/ProvenanceGraphBuilder.hpp"
#include "../include/parsing/FileParserFactory.hpp"
#include "../include/parsing/TSVFileParser.hpp"
#include "../include/parsing/ParsingStats.hpp"


using namespace std;
namespace bpa = fourbench::parsing;
namespace bpr = fourbench::provenance;
namespace bc = fourbench::conf;

const vector<string> testFilePaths {"input/test/test_kb.tsv", "input/test/test_kb2.tsv"};

BOOST_AUTO_TEST_CASE( fourbench_test_provenance_graph_distribution__uniform_nsources__perfile ) {
	bc::Conf& conf = bc::Conf::defaultConfig();
	bc::ConfValues& defaultConf = conf.getDefault();
	defaultConf.numberOfSources = bc::Conf::AUTO;
	defaultConf.sourcesDefinition = bc::SourcesDefinition::PERFILE; //This means 2 sources

	bpa::FileParserFactory factory = bpa::FileParserFactory::getInstance();
	bpa::TSVFileParser* tsvParser = factory.buildParser<bpa::TSVFileParser>(testFilePaths);
	bpa::ParsingStats stats = tsvParser->getParsingStats("default");

	BOOST_REQUIRE(stats.numberOfTriples == 9);
	BOOST_REQUIRE(stats.numberOfSubjects == 7);

	bpr::ProvenanceGraphBuilder builder = bpr::ProvenanceGraphBuilder::getInstance();
	bpr::ProvenanceGraph* graph = builder.buildProvenanceGraph(conf.getDefault(), stats);

	cout << conf << endl;
	cout << *graph << endl;

	BOOST_REQUIRE(graph->getNumberOfSourceEntities() == testFilePaths.size());
	BOOST_REQUIRE(graph->getNumberOfLeafEntities() == 5);
	BOOST_REQUIRE(graph->getNumberOfActivities() == 4);
	BOOST_REQUIRE(graph->getNumberOfIntermediateEntities() == 2);
	BOOST_REQUIRE(graph->getNumberOfEntities() == 9);

	delete graph;

}
