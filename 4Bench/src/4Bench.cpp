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
#include <algorithm>

#include "../include/conf/Conf.hpp"
#include "../include/parsing/FileParser.hpp"
#include "../include/parsing/FileParserFactory.hpp"
#include "../include/parsing/TSVFileParser.hpp"
#include "../include/output/ProvenanceDump.hpp"
#include "../include/output/ProvenanceDumpFactory.hpp"
#include "../include/output/QuadTSVProvenanceDump.hpp"
#include "../include/provenance/ProvenanceGraphFactory.hpp"
#include "../include/provenance/ProvenanceGraphPopulator.hpp"

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>

using namespace std;
namespace fc = fourbench::conf;
namespace po = boost::program_options;
namespace fpar = fourbench::parsing;
namespace fprov = fourbench::provenance;
namespace fo = fourbench::output;

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
	} else {
		cerr << "Format " << inputFormat << " is still not implemented";
		return nullptr;
	}
}

shared_ptr<fo::ProvenanceDump> getDump(const string& outputFormat, ostream& stream) {
	fo::ProvenanceDumpFactory& dumpFactory = fo::ProvenanceDumpFactory::getInstance();
	if (outputFormat == "tsv") {
		return dumpFactory.buildDump<fo::QuadTSVProvenanceDump>(stream);
	} else {
		cerr << "Format " << outputFormat << " is still not implemented";
		return nullptr;
	}
}

int main(int argc, char** argv) {
	po::variables_map vm;
	std::unique_ptr<po::options_description> description(readCmdArguments(argc, argv, &vm));
	fc::Conf &conf = fc::Conf::defaultConfig();
	string iformat;
	vector<string> ifiles;

	if(vm.count("help")) {
	    cout << *description;
		exit(0);
	}

	if (vm.count("version")) {
		cout << "0.1";
		exit(0);
	}

	if (vm.count("config")) {
		// Read it from file
		try {
			cout << "Config file: " << vm["config"].as<string>() << endl;
			conf.parseFromFile(vm["config"].as<string>());
		} catch (exception &e) {
			cout << e.what() << endl;
			exit(1);
		}
	} else {
		// Parse the other arguments
		conf.parseFromOptions(vm);
		cout << "Parsing of individual arguments is not yet implemented" << endl;
	}

	if (vm.count("input-files") == 0) {
		cerr << "No input files provided." << endl;
		exit(3);
	} else {
		ifiles = vm["input-files"].as<vector<string>>();
	}

	if (vm.count("input-format")) {
		iformat = vm["input-format"].as<string>();
		auto search = find(supportedInputFormats.begin(), supportedInputFormats.end(), iformat);
		if (search == end(supportedInputFormats)) {
			cerr << "Unsupported format " << iformat << ". This program only supports tsv, ttl, n3 or n4" << endl;
			exit(2);
		}
	} else {
		iformat = "tsv";
		cout << "Assuming tsv format for input files." << endl;
	}


	shared_ptr<fpar::FileParser> parser = buildParser(iformat, ifiles);
	if (parser.get() == nullptr) {
		exit(4);
	}

	cout << "-- Configuration --" << endl;
	cout << conf << endl;
	cout << "-- End of Configuration --" << endl;

	fprov::ProvenanceGraphFactory& builder = fprov::ProvenanceGraphFactory::getInstance();
	shared_ptr<map<string, shared_ptr<fprov::ProvenanceGraph>>> provenanceGraphs =
			builder.buildProvenanceGraphs(conf, parser->getAllParsingStats());

	for (auto itr = provenanceGraphs->begin(); itr != provenanceGraphs->end(); ++itr) {
		cout << *(itr->second) << endl;
	}

	ofstream outstream(vm["output"].as<string>());
	shared_ptr<fo::ProvenanceDump> dump = getDump(vm["output-format"].as<string>(), outstream);

	fprov::ProvenanceGraphPopulator populator(dump);
	populator.populate(*parser, *provenanceGraphs.get());

	return 0;
}
