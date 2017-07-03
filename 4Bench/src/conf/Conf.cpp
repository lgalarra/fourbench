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
#include <boost/program_options/variables_map.hpp>

#include "../include/conf/AssignmentDistribution.hpp"
#include "../include/conf/Conf.hpp"
#include "../include/utils/string.hpp"


using namespace std;
namespace po = boost::program_options;
using namespace fourbench;

namespace fourbench {
namespace conf {

	map<string, AssignmentDistribution> distributions = {{"uniform", UNIFORM},
			{"power_law", POWER_LAW},
			{"power", POWER_LAW},
			{"geometric", GEOMETRIC}};

	map<AssignmentDistribution, string> distributionsInv = {{UNIFORM, "uniform"},
				{POWER_LAW, "power_law"},
				{GEOMETRIC, "geometric"}};


	bool ConfValues::parseField(const string& fieldName, const string& value) {
		if (fieldName == "numberOfSources") {
			numberOfSources = stoul(value);
		} else if (fieldName == "metadataDepth") {
			metadataDepth = stoul(value);
		} else if (fieldName == "distribution") {
			if (distributions.count(value)) {
				distribution = distributions.at(value);
			} else {
				return false;
			}
		} else if (fieldName == "numberOfAgents") {
			numberOfAgents = stoul(value);
		} else if (fieldName == "activitiesDensity") {
			activitiesDensity = stof(value);
		} else if (fieldName == "activitiesEntitiesDensity") {
			activitiesEntitiesDensity = stof(value);
		} else if (fieldName == "agentsEntitiesDensity") {
			agentsEntitiesDensity = stof(value);
		} else if (fieldName == "properties") {
			vector<string> inputProperties = split(value, ",");
			for_each(inputProperties.begin(), inputProperties.end(), fourbench::trim);
			properties.insert(inputProperties.begin(), inputProperties.end());
		} else {
			return false;
		}

		return true;
	}

	bool ConfValues::isDefaultProperty() {
		return properties.empty();
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

	void Conf::indexProperties() {
		for (auto itr = confs.begin(); itr != confs.end(); ++itr) {
			string family = itr->first;
			if (family != "default") {
				set<string>& properties = itr->second->properties;
				for (auto itrProp = properties.begin(); itrProp != properties.end(); ++itrProp) {
					properties2Families[*itrProp] = family;
				}
			}
		}
	}


	Conf& Conf::defaultConfig() {
		return Conf::defaultInstance;
	}

	bool Conf::parseFromFile(const string& filename) throw() {
		ifstream iniFile(filename);
		if (iniFile.good()) {
			map<string, string> vm;
			po::options_description description("");
			string line;
			unsigned lineNumber = 1;
			while (std::getline(iniFile, line)) {
			    std::istringstream iss(line);
			    vector<string> parts = split(line, "=");
			    if (parts.size() > 1) {
					string key = trim(parts[0]);
					string value = trim(parts[1]);
					vm[key] = value;
			    } else {
			    	cerr << "Error at parsing line " << lineNumber << ": " << line;
			    }
			    ++lineNumber;
			}
			bool returnValue = this->parseFromOptions(vm);
			this->indexProperties();
			return returnValue;
		} else {
			cerr << "There was a problem at opening file " << filename << endl;
			return false;
		}
	}

	bool Conf::parseFromOptions(const po::variables_map& vm) {
		map<string, string> stdmap;
		for (auto itr = vm.begin(); itr != vm.end(); ++itr) {
			stdmap[itr->first] = itr->second.as<string>();
		}
		bool returnValue = this->parseFromOptions(stdmap);
		this->indexProperties();
		return returnValue;
	}

	bool Conf::parseFromOptions(const map<string, string>& vm) {
		bool answer = true;
		for (auto itr = vm.begin(); itr != vm.end(); ++itr) {
			vector<string> parts = split(itr->first, ":");
			string family, fieldName;
			if (parts.size() > 1) {
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

			if (!vals->parseField(fieldName, itr->second)) {
				cerr << "Field " << fieldName << " is not recognized." << endl;
				answer = false;
			}
		}

		return answer;

	}

	string Conf::getFamily(string& property) {
		if (properties2Families.find(property) != properties2Families.end()) {
			return this->properties2Families[property];
		} else {
			return "default";
		}
	}


	ostream& operator<<(ostream& stm, const set<string>& v) {
		stm << "[";
		for (auto itr = v.begin(); itr != v.end(); ++itr) {
			stm << *itr << " ";
		}
		stm << "]";
		return stm;
	}

	ostream& operator<<(ostream &strm, const ConfValues &a) {
		strm << "Conf(numberOfSources: " << a.numberOfSources << ", ";
		strm << "metadataDepth: " << a.metadataDepth << ", ";
		strm << "distribution: " << distributionsInv[a.distribution] << ", ";
		strm << "numberOfAgents: " << a.numberOfAgents << ", ";
		strm << "activitiesDensity: " << a.activitiesDensity << ", ";
		strm << "activitiesEntitiesDensity: " << a.activitiesEntitiesDensity << ", ";
		strm << "agentsEntitiesDensity: " << a.agentsEntitiesDensity << ", ";
		if (a.properties.empty()) {
			strm << "properties: *";
		} else {
			strm << "properties: " << a.properties;
		}
		strm << ")";
		return strm;
	}

	ostream& operator<<(ostream &strm, const Conf& a) {
		for (auto itr = a.confs.begin(); itr != a.confs.end(); itr++) {
			strm << itr->first << ": " << *(itr->second) << "; ";
		}
		return strm;
	}

}
}
