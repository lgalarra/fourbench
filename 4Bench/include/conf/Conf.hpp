/*
 * Conf.hpp
 *
 * It defines the configuration parameters of the provenance generation tool.
 *
 * Created on: Jun 19, 2017
 *      Author: galarraga
 */

#ifndef CONF_CONF_HPP_
#define CONF_CONF_HPP_

#include <exception>
#include <set>
#include <map>
#include <string>
#include <ostream>

#include <boost/program_options.hpp>
#include <boost/program_options/variables_map.hpp>

#include "AssignmentDistribution.hpp"

using namespace std;
namespace po = boost::program_options;


namespace fourbench {
namespace conf {

struct ConfValues {
	unsigned numberOfSources;
	unsigned metadataDepth;
	AssignmentDistribution distribution;
	unsigned numberOfAgents;
	float activitiesDensity;
	float activitiesEntitiesDensity;
	float agentsEntitiesDensity;
	string familyName;
	set<string> properties;

	friend struct Conf;

	bool isDefaultProperty();

private:
	bool parseField(const string& fieldName, const string& value);

};

typedef struct ConfValues ConfValues;

ostream& operator<<(ostream &strm, const ConfValues &a);

struct Conf {
private:
	map<string, string> properties2Families;
	static Conf defaultInstance;

	void indexProperties();
	Conf();
	bool parseFromOptions(const map<string, string>& vm);

public:
	static const unsigned DefaultNumberOfSources = 10;
	static const unsigned DefaultDepth = 2;
	static const unsigned DefaultNumberOfAgents = 3;
	static constexpr float DefaultActivitiesDensity = 0.5f;
	static constexpr float DefaultActivitiesEntitiesDensity = 0.5f;
	static constexpr float DefaultAgentsEntitiesDensity = 0.5f;

	map<string, ConfValues*> confs;

	static Conf& defaultConfig();
	bool parseFromFile(const string& filename) throw();
	bool parseFromOptions(const po::variables_map& vm);
	string getFamily(string& property);
	virtual ~Conf();

};

typedef struct Conf Conf;

ostream& operator<<(ostream&, const Conf&);

ostream& operator<<(ostream& stm, const set<string>& v);


}
}

#endif /* CONF_HPP_ */
