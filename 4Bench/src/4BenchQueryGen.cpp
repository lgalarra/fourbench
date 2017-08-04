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
	storage = librdf_new_storage(world, sengine.c_str(), "storage", NULL);
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

	vector<string> queries = loadQueries(iqueries);

	for (auto qit = queries.begin(); qit != queries.end(); ++qit) {
		// Do something with the query
	}

/**	rasqal_query_results *results;
	rasqal_query *rq=rasqal_new_query(world,"sparql",NULL);
	const unsigned char query_string[] = "PREFIX rdf: <http://www.w3.org/1999/02/22-rdf-syntax-ns#> "
			"PREFIX foaf: <http://xmlns.com/foaf/0.1/> SELECT DISTINCT ?name WHERE { { ?x rdf:type foaf:Person . ?x foaf:name ?name . } UNION { ?x foaf:nickname ?name } } "
			"ORDER BY ?name";
	FILE* f =  fopen("q", "w");
	rasqal_query_prepare(rq,query_string, NULL);
	rasqal_graph_pattern *gp = rasqal_query_get_query_graph_pattern(rq);
	rasqal_graph_pattern_print(gp, f);
	rasqal_graph_pattern* sgp  = rasqal_graph_pattern_get_sub_graph_pattern(gp, 0);
	rasqal_triple *t =  rasqal_graph_pattern_get_triple(sgp, 0);
	rasqal_triple_print(t, f);
	cout << rasqal_literal_as_string(t->object) << endl;
	fclose(f);**/

/**	rasqal_free_query(rq);
	rasqal_free_world(world);
	raptor_free_world(w);**/
	librdf_free_world(world);
	librdf_free_model(model);
	librdf_free_storage(storage);

	/* keep gcc -Wall happy */
	return(0);
}
