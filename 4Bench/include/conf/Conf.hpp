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

#include "AssignmentDistribution.hpp"

using namespace std;
namespace po = boost::program_options;


namespace fourbench {

struct ConfValues {
	unsigned numberOfSources;
	unsigned metadataDepth;
	AssignmentDistribution distribution;
	unsigned numberOfAgents;
	float activitiesDensity;
	float activitiesEntitiesDensity;
	float agentsEntitiesDensity;
	string familyName;

	friend struct Conf;

private:
	bool parseField(const string& fieldName, const po::variable_value& value);

};

typedef struct ConfValues ConfValues;

std::ostream& operator<<(std::ostream &strm, const ConfValues &a);

struct Conf {
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
	bool parseFromOptions(po::variables_map& vm);
	virtual ~Conf();

private:
	static Conf defaultInstance;

	Conf();
};

typedef struct Conf Conf;

std::ostream& operator<<(std::ostream&, const Conf&);


}

#endif /* CONF_HPP_ */
