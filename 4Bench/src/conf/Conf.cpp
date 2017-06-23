/*
 * Conf.cpp
 *
 *  Created on: Jun 19, 2017
 *      Author: galarraga
 */


#include <memory>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <string>
#include <boost/program_options.hpp>

#include "../include/conf/AssignmentDistribution.hpp"
#include "../include/conf/Conf.hpp"
#include "../include/utils/string.hpp"


using namespace std;
namespace po = boost::program_options;


namespace fourbench {

	map<string, AssignmentDistribution> distributions = {{"uniform", UNIFORM},
			{"power_law", POWER_LAW},
			{"power", POWER_LAW},
			{"geometric", GEOMETRIC}};

	map<AssignmentDistribution, string> distributionsInv = {{UNIFORM, "uniform"},
				{POWER_LAW, "power_law"},
				{GEOMETRIC, "geometric"}};


	bool ConfValues::parseField(const string& fieldName, const po::variable_value& value) {
		if (fieldName == "numberOfSources") {
			numberOfSources = value.as<unsigned>();
		} else if (fieldName == "metadataDepth") {
			metadataDepth = value.as<unsigned>();
		} else if (fieldName == "distribution") {
			if (distributions.count(value.as<string>())) {
				distribution = distributions.at(value.as<string>());
			} else {
				return false;
			}
		} else if (fieldName == "numberOfAgents") {
			numberOfAgents = value.as<unsigned>();
		} else if (fieldName == "activitiesDensity") {
			activitiesDensity = value.as<float>();
		} else if (fieldName == "activitiesEntitiesDensity") {
			activitiesEntitiesDensity = value.as<float>();
		} else if (fieldName == "agentsEntitiesDensity") {
			agentsEntitiesDensity = value.as<float>();
		} else {
			return false;
		}

		return true;
	}


	Conf Conf::defaultInstance;

	Conf::Conf() {
		ConfValues* values = new ConfValues();
		values->numberOfSources = Conf::DefaultNumberOfSources;
		values->metadataDepth = Conf::DefaultDepth;
		values->distribution = UNIFORM;
		values->numberOfAgents = Conf::DefaultNumberOfAgents;
		values->activitiesDensity = Conf::DefaultActivitiesDensity;
		values->activitiesEntitiesDensity = Conf::DefaultActivitiesEntitiesDensity;
		values->agentsEntitiesDensity = Conf::DefaultAgentsEntitiesDensity;
		confs["default"] = values;
	}

	Conf::~Conf() {
		for_each (confs.begin(), confs.end(), [](pair<string, ConfValues*> item) -> void
		{
		  delete item.second;
		});
	}

	Conf& Conf::defaultConfig() {
		return Conf::defaultInstance;
	}

	bool Conf::parseFromFile(const string& filename) throw() {
		ifstream iniFile(filename);
		if (iniFile.good()) {
			po::variables_map vm;
			po::options_description description("");
			po::store(po::parse_config_file(iniFile, description, true), vm);
			return Conf::parseFromOptions(vm);
		} else {
			cerr << "There was a problem at opening file " << filename << endl;
			return false;
		}
	}

	bool Conf::parseFromOptions(po::variables_map& vm) {
		bool answer = true;
		for (auto itr = vm.begin(); itr != vm.end(); ++itr) {
			vector<string> parts = split(itr->first, ":");
			string family, fieldName;
			if (parts.size() > 2) {
				family = parts[0];
				fieldName = parts[1];
			} else if (parts.size() == 1) {
				fieldName = parts[0];
			}

			if (family.empty()) {
				family = "default";
			}

			ConfValues* vals = nullptr;
			if (confs.count(family) == 0) {
				vals = new ConfValues();
				vals->familyName = family;
				confs[family] = vals;
			} else {
				vals = confs[family];
			}

			if (vals->parseField(fieldName, itr->second)) {
				cerr << "Field " << fieldName << " is not recognized." << endl;
				answer = false;
			}
		}

		return answer;

	}

	std::ostream& operator<<(std::ostream &strm, const ConfValues &a) {
		strm << "Conf(numberOfSources: " << a.numberOfSources << ", ";
		strm << "metadataDepth: " << a.metadataDepth << ", ";
		strm << "distribution: " << distributionsInv[a.distribution] << ", ";
		strm << "numberOfAgents: " << a.numberOfAgents << ", ";
		strm << "activitiesDensity: " << a.activitiesDensity << ", ";
		strm << "activitiesEntitiesDensity: " << a.activitiesEntitiesDensity << ", ";
		strm << "agentsEntitiesDensity: " << a.agentsEntitiesDensity;
		strm << ")";
		return strm;
	}

	std::ostream& operator<<(std::ostream &strm, const Conf &a) {
		for (auto itr = a.confs.begin(); itr != a.confs.end(); itr++) {
			strm << itr->first << ": " << *(itr->second) << "; ";
		}
		return strm;
	}


}
