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
#include "SourcesDefinition.hpp"

using namespace std;
namespace po = boost::program_options;


namespace fourbench {
namespace conf {

struct ConfValues {
	unsigned numberOfSources;
	unsigned metadataDepth;
	AssignmentDistribution distribution;
	SourcesDefinition sourcesDefinition;
	unsigned numberOfAgents;
	float sources2LeavesDensity;
	float triplesEntitiesDensity;
	float activitiesDensity;
	float activitiesEntitiesDensity;
	unsigned maxNumberOfAgentsPerActivity;
	unsigned maxNumberOfAgentsPerSourceEntity;
	unsigned maxNumberOfAttributes;
	bool provenancePerSubject;
	string familyName;
	set<string> properties;

	friend struct Conf;

	ConfValues();
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
	Conf(const Conf&) = delete;
	bool parseFromOptions(const map<string, string>& vm);

public:
	static const unsigned DefaultNumberOfSources = 10;
	static const unsigned DefaultDepth = 2;
	static const unsigned DefaultNumberOfAgents = 3;
	static constexpr float DefaultTriplesEntitiesDensity = 0.5f;
	static constexpr float DefaultSources2LeavesDensity = 0.5f;
	static constexpr float DefaultActivitiesDensity = 0.5f;
	static constexpr float DefaultActivitiesEntitiesDensity = 0.5f;
	static const unsigned DefaultMaxNumberOfAgentsPerSourceEntity = 1;
	static const unsigned DefaultMaxNumberOfAgentsPerActivity = 1;
	static const unsigned DefaultMaxNumberOfAttributesForEntities = 5;
	static const unsigned AUTO = 0;

	map<string, ConfValues*> confs;

	static Conf& defaultConfig();
	bool parseFromFile(const string& filename) throw();
	bool parseFromOptions(const po::variables_map& vm);
	ConfValues& getDefault();
	ConfValues& get(const string& family);
	string getFamily(const string& property);
	virtual ~Conf();

};

typedef struct Conf Conf;

ostream& operator<<(ostream&, const Conf&);

ostream& operator<<(ostream& stm, const set<string>& v);


}
}

#endif /* CONF_HPP_ */
