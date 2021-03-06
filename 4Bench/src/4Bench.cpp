/*
 ============================================================================
 Name        : 4Bench.cpp
 Author      : Luis Galárraga
 Version     :
 Copyright   : 
 Description : 4Bench main program
 ============================================================================
 */

#include <iostream>
#include <ostream>
#include <fstream>
#include <memory>
#include <vector>
#include <chrono>
#include <algorithm>

#include "../include/utils/time.hpp"
#include "../include/4bench.hpp"
#include "../include/conf/Conf.hpp"
#include "../include/parsing/FileParser.hpp"
#include "../include/parsing/FileParserFactory.hpp"
#include "../include/parsing/TSVFileParser.hpp"
#include "../include/parsing/N3FileParser.hpp"
#include "../include/output/ProvenanceDump.hpp"
#include "../include/output/ProvenanceDumpFactory.hpp"
#include "../include/output/NQuadsProvenanceDump.hpp"
#include "../include/provenance/ProvenanceGraphFactory.hpp"
#include "../include/provenance/ProvenanceGraphPopulator.hpp"
#include "../include/provenance/PopulateStats.hpp"

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>

using namespace std;
namespace fc = fourbench::conf;
namespace po = boost::program_options;
namespace fpar = fourbench::parsing;
namespace fprov = fourbench::provenance;
namespace fo = fourbench::output;
namespace f = fourbench;
using sc = chrono::steady_clock;

const vector<string> supportedInputFormats = {"tsv", "ttl", "n3", "n4"};

po::options_description* readCmdArguments(int argc, char** argv, po::variables_map* optionsMap) {
	po::options_description *description = new po::options_description("4Bench Usage");
	po::positional_options_description posDescription;

	description->add_options()
	    ("help,h", "Display this help message")
	    ("version,v", "Display the version number")
		("config,c", po::value<string>(), "Configuration file (.ini)")
		("input-files,F", po::value<vector<string>>()->composing(), "Input files")
		("input-format,f", po::value<string>()->default_value("tsv"), "Format for input files (tsv, ttl, n3, n4)")
		("output-format,t", po::value<string>()->default_value("tsv"), "Format for output file (tsv, ttl, n3, n4)")
		("output,o", po::value<string>()->default_value("4bench_output"), "Output file");

	posDescription.add("input-files", -1);
	po::store(po::command_line_parser(argc, argv).positional(posDescription).options(*description).run(), *optionsMap);
	po::notify(*optionsMap);
	return description;
}

shared_ptr<fpar::FileParser> buildParser(const string& inputFormat, const vector<string>& inputFiles) {
	fpar::FileParserFactory& parserFactory = fpar::FileParserFactory::getInstance();
	if (inputFormat == "tsv") {
		return parserFactory.buildParser<fpar::TSVFileParser>(inputFiles);
	} else if (inputFormat == "n3" || inputFormat == "ntriples") {
		return parserFactory.buildParser<fpar::N3FileParser>(inputFiles);
	} else {
		cerr << "Format " << inputFormat << " is still not implemented";
		return nullptr;
	}
}

shared_ptr<fo::ProvenanceDump> getDump(const string& outputFormat, ostream& stream) {
	fo::ProvenanceDumpFactory& dumpFactory = fo::ProvenanceDumpFactory::getInstance();
	if (outputFormat == "nquads" || outputFormat == "n4") {
		return dumpFactory.buildDump<fo::NQuadsProvenanceDump>(stream);
	} else {
		return nullptr;
	}
}

void outputDumpStats(const fo::ProvenanceDump& dump) {
	cout << "Dump Stats { #provenance-triples: " << dump.getNumberOfDumpedProvenanceTriples();
	cout << ", #output-triples: " << dump.getNumberOfDumpedTriples() << "}" << endl;
}

int main(int argc, char** argv) {
	po::variables_map vm;
	std::unique_ptr<po::options_description> description(readCmdArguments(argc, argv, &vm));
	fc::Conf &conf = fc::Conf::defaultConfig();
	string iformat;
	vector<string> ifiles;

	if(vm.count("help")) {
	    cout << *description;
		return 0;
	}

	if (vm.count("version")) {
		cout << FOURBENCH_VERSION;
		return 0;
	}

	if (vm.count("config")) {
		// Read it from file
		try {
			cout << "Config file: " << vm["config"].as<string>() << endl;
			conf.parseFromFile(vm["config"].as<string>());
		} catch (exception &e) {
			cout << e.what() << endl;
			return 1;
		}
	} else {
		// Parse the other arguments
		conf.parseFromOptions(vm);
		cout << "Parsing of individual arguments is not yet implemented" << endl;
	}

	cout << "-- Configuration --" << endl;
	cout << conf << endl;
	cout << "-- End of Configuration --" << endl;

	if (vm.count("input-files") == 0) {
		cerr << "No input files provided." << endl;
		return 3;
	} else {
		ifiles = vm["input-files"].as<vector<string>>();
	}

	if (vm.count("input-format")) {
		iformat = vm["input-format"].as<string>();
		auto search = find(supportedInputFormats.begin(), supportedInputFormats.end(), iformat);
		if (search == end(supportedInputFormats)) {
			cerr << "Unsupported format " << iformat << ". This program only supports tsv, ttl, n3 or n4" << endl;
			return 2;
		}
	} else {
		iformat = "tsv";
		cout << "Assuming tsv format for input files." << endl;
	}

	cout << "Initialization (first pass on the data) started." << endl;
	sc::time_point tstart = f::time();
	shared_ptr<fpar::FileParser> parser = buildParser(iformat, ifiles);
	if (parser.get() == nullptr) {
		exit(4);
	}
	sc::time_point tend = f::time();
	cout << "Initialization took " << f::formatElapsedTime(tstart, tend) << endl;

	fprov::ProvenanceGraphFactory& builder = fprov::ProvenanceGraphFactory::getInstance();
	shared_ptr<map<string, shared_ptr<fprov::ProvenanceGraph>>> provenanceGraphs =
			builder.buildProvenanceGraphs(conf, parser->getAllParsingStats());

#ifdef DEBUG
	for (auto itr = provenanceGraphs->begin(); itr != provenanceGraphs->end(); ++itr) {
		cout << *(itr->second) << endl;
	}
#endif

	ofstream outstream(vm["output"].as<string>());

	shared_ptr<fo::ProvenanceDump> dump = getDump(vm["output-format"].as<string>(), outstream);
	fprov::ProvenanceGraphPopulator populator(dump);
	tstart = f::time();
	shared_ptr<map<string, shared_ptr<fprov::PopulateStats>>> popStats = populator.populate(parser, provenanceGraphs);
	tend = f::time();
	cout << "Output of provenance graph took " << f::formatElapsedTime(tstart, tend) << endl;
	cout << "Data generation statistics" << endl;
	for (auto itr = popStats->begin(); itr != popStats->end(); ++itr) {
		cout << *(itr->second.get()) << endl;
	}

	outputDumpStats(*dump.get());

	outstream.close();

	return 0;
}
