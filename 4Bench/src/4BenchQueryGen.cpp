/*
 * 4BenchQueryGen.cpp
 *
 *  Created on: Aug 3, 2017
 *      Author: galarraga
 */


#include <iostream>
#include <sstream>
#include <fstream>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdarg.h>
#include <vector>
#include <string>

#include <rasqal.h>
#include <raptor2.h>
#include <redland.h>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>

#include "../include/4bench.hpp"
#include "../include/utils/string.hpp"

using namespace std;
namespace po = boost::program_options;
namespace f = fourbench;

po::options_description* readCmdArguments(int argc, char** argv, po::variables_map* optionsMap) {
	po::options_description *description = new po::options_description("4BenchQueryGen Usage");
	po::positional_options_description posDescription;

	description->add_options()
	    ("help,h", "Display this help message")
	    ("version,v", "Display the version number")
		("dataset,d", po::value<vector<string>>()->composing(), "Dataset")
		("input-files,i", po::value<vector<string>>()->composing(), "Input files")
		("input-format,f", po::value<string>()->default_value("ntriples"), "Format for input files (tsv, turtle, ntriples, nquads)")
		("storage-engine,s", po::value<string>()->default_value("memory"), "Storage method: memory, bdb (Berkeley DB), sqlite, mysql, postgres")
		("output,o", po::value<string>()->default_value("4benchquerygen_output"), "Output file");
		("output-format,t", po::value<string>()->default_value("sparql+graphs"), "Output format: sparql, sparql+graphs, sparql*");

	posDescription.add("input-files", -1);
	po::store(po::command_line_parser(argc, argv).positional(posDescription).options(*description).run(), *optionsMap);
	po::notify(*optionsMap);
	return description;
}


bool loadData(const string& iformat, librdf_world* world, librdf_model* model, const vector<string>& idataset) {
	const char* istrformat = iformat.c_str();
	for (auto it = idataset.begin(); it != idataset.end(); ++it) {
		string uriPath = f::concat({"file:", *it});
		librdf_parser* parser = librdf_new_parser(world, istrformat, NULL, NULL);
		if (parser == NULL) {
			cerr << "Failed to create new parser " << iformat << endl;
			return false;
		}

		librdf_uri* uri = librdf_new_uri(world, (const unsigned char*)uriPath.c_str());
		cout << "Parsing uri " << uriPath << endl;
		if (uri == NULL) {
			cerr << "Failed to create URI " << istrformat << endl;
			librdf_free_parser(parser);
			return false;
		}

		if (librdf_parser_parse_into_model(parser, uri, uri, model)) {
			cerr << "Failed to parse RDF into model" << endl;
			librdf_free_uri(uri);
			librdf_free_parser(parser);
			return false;
		}

		librdf_free_uri(uri);
		librdf_free_parser(parser);
	}
	return true;
}

vector<string> loadQueries(const vector<string>& ifiles) {
	vector<string> result;
	for (auto it = ifiles.begin(); it != ifiles.end(); ++it) {
		std::ifstream t(*it);
		if (t.good()) {
			std::stringstream buffer;
			buffer << t.rdbuf();
			result.push_back(buffer.str());
		} else {
			cerr << "Problem reading file " << *it << endl;
		}
	}

	return result;
}

int
main(int argc, char *argv[])
{
	librdf_world* world;
	librdf_model* model;
	librdf_storage* storage;
	po::variables_map vm;
	std::unique_ptr<po::options_description> description(readCmdArguments(argc, argv, &vm));
	string iformat("ntriples");
	string sengine("memory");
	vector<string> idataset, iqueries;

	if(vm.count("help")) {
	    cout << *description;
		return 0;
	}

	if (vm.count("version")) {
		cout << FOURBENCH_VERSION;
		return 0;
	}

	if (vm.count("input-format")) {
		iformat = vm["input-format"].as<string>();
		cout << "Using input format " << iformat << endl;
	}

	if (vm.count("storage-engine")) {
		sengine = vm["storage-engine"].as<string>();
		cout << "Using storage engine " << sengine << endl;
	}

	if (vm.count("dataset") == 0) {
		cerr << "No input dataset provided" << endl;
		return 2;
	} else {
		idataset = vm["dataset"].as<vector<string>>();
	}

	if (vm.count("input-files") == 0) {
		cerr << "No input queries provided" << endl;
		return 2;
	} else {
		iqueries = vm["input-files"].as<vector<string>>();
	}


	world = librdf_new_world();
	storage = librdf_new_storage(world, sengine.c_str(), "storage", "hash-type='memory',dir='.',contexts='no'");
	if (storage == NULL) {
		cerr << "Failed to create new storage of type " << sengine << endl;
	    return 1;
	}

	model = librdf_new_model(world, storage, NULL);
	if (model == NULL) {
		cerr << "Failed to create model" << endl;
	    return 1;
	}

	if (!loadData(iformat, world, model, idataset)) {
		cerr << "Error at parsing the input data" << endl;
		return 2;
	}

	cout << librdf_model_size(model) << " statements loaded " << endl;


	vector<string> queries = loadQueries(iqueries);

	for (auto qit = queries.begin(); qit != queries.end(); ++qit) {
		librdf_statement* partial_statement = librdf_new_statement(world);
		librdf_node* subject=librdf_new_node_from_uri_string(world, (const unsigned char*)"http://example.com/Quito");
		librdf_node* predicate=librdf_new_node_from_uri_string(world, (const unsigned char*)"http://example.com/capitalOf");
		librdf_statement_set_subject(partial_statement, subject);
		librdf_statement_set_predicate(partial_statement, predicate);
		librdf_stream* stream = librdf_model_find_statements(model, partial_statement);
		while(!librdf_stream_end(stream)) {
		      librdf_statement *statement=librdf_stream_get_object(stream);
		      if(!statement) {
		        cerr << "Error" << endl;
		        break;
		      }

		      fputs("  Matched statement: ", stdout);
		      librdf_statement_print(statement, stdout);
		      fputc('\n', stdout);

		      librdf_stream_next(stream);
		      //count++;
		}
		librdf_free_stream(stream);
		librdf_free_node(subject);
		librdf_free_node(predicate);
		librdf_free_statement(partial_statement);
	}

	librdf_free_world(world);
	librdf_free_model(model);
	librdf_free_storage(storage);

	/* keep gcc -Wall happy */
	return(0);
}
