/*
 * TSVFileParserTest.cpp
 * Simple test for the
 *  Created on: Jun 30, 2017
 *      Author: galarraga
 */


#define BOOST_TEST_MODULE fourbench_test_tsvparser



#include <fstream>
#include <iostream>
#include <boost/test/included/unit_test.hpp>

#include "../include/parsing/FileParserFactory.hpp"
#include "../include/parsing/TSVFileParser.hpp"
#include "../include/parsing/ParsingStats.hpp"


using namespace std;
namespace bp = fourbench::parsing;

const vector<string> testFilePaths {"input/test/test_kb.tsv", "input/test/test_kb2.tsv"};

const vector<string> singletestFilePath {"input/test/test_kb.tsv"};

bool canReadTestFiles(const vector<string>& paths) {
	for (auto itr = paths.begin(); itr != paths.end(); ++itr) {
		ifstream testFile(*itr);
		if (!testFile.good())
			return false;
	}

	return true;
}

BOOST_AUTO_TEST_CASE( fourbench_test_tsvparser )
{
	// Check whether we can find the test file
	BOOST_REQUIRE(canReadTestFiles(testFilePaths));
	bp::FileParserFactory factory = bp::FileParserFactory::getInstance();

	bp::TSVFileParser* tsvParser = factory.buildParser<bp::TSVFileParser>(testFilePaths);
	bp::ParsingStats stats = tsvParser->getParsingStats("default");
	BOOST_REQUIRE(stats.numberOfTriples == 9);
	BOOST_REQUIRE(stats.numberOfSubjects == 7);

	delete tsvParser;

}

BOOST_AUTO_TEST_CASE( fourbench_test_single_tsvparser )
{
	// Check whether we can find the test file
	BOOST_REQUIRE(canReadTestFiles(singletestFilePath));
	bp::FileParserFactory factory = bp::FileParserFactory::getInstance();

	bp::TSVFileParser* tsvParser = factory.buildParser<bp::TSVFileParser>(singletestFilePath);
	bp::ParsingStats stats = tsvParser->getParsingStats("default");

	BOOST_REQUIRE(stats.numberOfTriples == 7);
	BOOST_REQUIRE(stats.numberOfSubjects == 5);

	delete tsvParser;


}



