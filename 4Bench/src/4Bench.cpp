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
#include <memory>

#include "../include/conf/Conf.hpp"

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>

using namespace std;
using namespace fourbench::conf;
namespace po = boost::program_options;

po::options_description* readCmdArguments(int argc, char** argv, po::variables_map* optionsMap) {
	po::options_description *description = new po::options_description("4Bench Usage");

	description->add_options()
	    ("help,h", "Display this help message")
	    ("version,v", "Display the version number")
		("config,c", po::value<string>(), "Configuration file (.ini)");

	po::store(po::command_line_parser(argc, argv).options(*description).run(), *optionsMap);
	po::notify(*optionsMap);
	return description;
}

int main(int argc, char** argv) {
	po::variables_map vm;
	std::unique_ptr<po::options_description> description(readCmdArguments(argc, argv, &vm));
	Conf &conf = Conf::defaultConfig();

	if(vm.count("help")) {
	    cout << *description;
		exit(1);
	}

	if (vm.count("version")) {
		cout << "0.1";
		exit(1);
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

	cout << conf << endl;
	return 0;
}
