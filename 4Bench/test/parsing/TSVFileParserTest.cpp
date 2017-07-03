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


using namespace std;
namespace bp = fourbench::parsing;

const string testFilePath = "input/test/test_kb.tsv";

bool canReadTestFile() {
	ifstream testFile(testFilePath);
	return testFile.good();
}

BOOST_AUTO_TEST_CASE( fourbench_test_tsvparser )
{
	// Check whether we can find the test file
	BOOST_REQUIRE(canReadTestFile());
	bp::FileParserFactory factory = bp::FileParserFactory::getInstance();

	bp::TSVFileParser* tsvParser = factory.buildParser<bp::TSVFileParser>(testFilePath);

	cout << tsvParser->getNumberOfTriples();
	BOOST_REQUIRE(tsvParser->getNumberOfTriples() == 7);

	delete tsvParser;


}


