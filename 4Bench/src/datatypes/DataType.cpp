/*
 * DataType.cpp
 *
 *  Created on: Jul 24, 2017
 *      Author: galarraga
 */

#include <memory>
#include <time.h>
#include <stdio.h>
#include <fstream>

#include "../include/conf/Conf.hpp"
#include "../include/provenance/IRIType.hpp"
#include "../include/provenance/IRIBuilder.hpp"
#include "../include/utils/float.hpp"
#include "../include/utils/integer.hpp"
#include "../include/datatypes/DataType.hpp"

using namespace std;

namespace fprov = fourbench::provenance;
namespace fc = fourbench::conf;
namespace f = fourbench;

namespace fourbench {
namespace datatypes {

DataType::DataType(const string& name) : typeName(name) {

}

DataType::~DataType() {}

string DataType::getName() const {
	return typeName;
}

IntegerType IntegerType::instance;

IntegerType& IntegerType::getInstance() {
	return instance;
}

IntegerType::IntegerType() : DataType("integer") {

}

IntegerType::~IntegerType() {

}

shared_ptr<DataValue> IntegerType::getRandomValue() const {
	return make_shared<IntegerValue>(rand());
}

BooleanType BooleanType::instance;

BooleanType& BooleanType::getInstance() {
	return instance;
}


BooleanType::BooleanType() : DataType("boolean") {

}

shared_ptr<DataValue> BooleanType::getRandomValue() const {
	return make_shared<BooleanValue>(f::urand(0, 2) == 0);
}

BooleanType::~BooleanType() {

}

RatioType RatioType::instance;

RatioType& RatioType::getInstance() {
	return instance;
}

RatioType::RatioType() : DataType("ratio") {

}

shared_ptr<DataValue> RatioType::getRandomValue() const {
	return make_shared<RatioValue>(f::frrand());
}

RatioType::~RatioType() {

}

DateTimeType DateTimeType::instance;

DateTimeType::DateTimeType(const string& name) : DataType(name) {}

DateTimeType::DateTimeType() : DataType("dateTime") {}

shared_ptr<DataValue> DateTimeType::getRandomValue() const {
	long rtimestamp = random();
	time_t rDate = time(&rtimestamp);
	return make_shared<DateTimeValue>(rDate);
}

DateTimeType::~DateTimeType() {}

DateTimeType& DateTimeType::getInstance() {
	return instance;
}

DateType DateType::instance;

DateType& DateType::getInstance() {
	return instance;
}


DateType::DateType() : DateTimeType("date") {

}

shared_ptr<DataValue> DateType::getRandomValue() const {
	long rtimestamp = random();
	time_t rDate = time(&rtimestamp);
	return make_shared<DateValue>(rDate);
}

DateType::~DateType() {

}

EnumeratedDomain::EnumeratedDomain(const string& filename) {
	ifstream iniFile(filename);
	if (iniFile.good()) {
		string line;
		unsigned lineNumber = 1;
		while (getline(iniFile, line)) {
		    values.insert(line);
		}
	} else {
		values.insert("something went wrong");
	}
}

string EnumeratedDomain::getRandomValue() const {
	int randPos = rand() % values.size();
	auto it = values.begin();
	advance(it, randPos);
	return *it;
}

EnumeratedDomain::~EnumeratedDomain() {}

StringType StringType::instance;

StringType::StringType() : DataType("string"), EnumeratedDomain("input/domains/strings") {

}

StringType& StringType::getInstance() {
	return instance;
}

shared_ptr<DataValue> StringType::getRandomValue() const {
	return make_shared<StringValue>(EnumeratedDomain::getRandomValue());
}

StringType::~StringType() {

}

IRIType IRIType::instance;

IRIType& IRIType::getInstance() {
	return instance;
}


IRIType::IRIType() : DataType("IRI") {

}

IRIType::IRIType(const string& name) : DataType(name) {

}

shared_ptr<DataValue> IRIType::getRandomValue() const {
	fprov::IRIType iriType = static_cast<fprov::IRIType>(f::urand(0, 3));
	unsigned id;
	fc::ConfValues defaultValues = fc::Conf::defaultConfig().get("default");

	switch (iriType) {
	case fprov::AGENT :
		id = f::urand(0, defaultValues.numberOfAgents);
		break;
	case fprov::ENTITY : case fprov::ACTIVITY :
		id = f::urand(0, 10);
		break;
	}
	shared_ptr<fprov::IRIBuilder> iriBuilder = fprov::IRIBuilder::getInstance(fprov::IRIBuilder::getDefaultDomain());
	string iri = iriBuilder->getIRI(iriType, id);
	return make_shared<IRIValue>(iri);
}

IRIType::~IRIType() {

}

CountryType CountryType::instance;

CountryType& CountryType::getInstance() {
	return instance;
}


CountryType::CountryType() : IRIType("country"), EnumeratedDomain("input/domains/countries") {

}

shared_ptr<DataValue> CountryType::getRandomValue() const {
	return make_shared<CountryValue>(EnumeratedDomain::getRandomValue());
}

CountryType::~CountryType() {

}

AgentType AgentType::instance;

AgentType& AgentType::getInstance() {
	return instance;
}


AgentType::AgentType() : DataType("agentType") {

}

shared_ptr<DataValue> AgentType::getRandomValue() const {
	return make_shared<AgentTypeValue>(static_cast<AgentTypeEnum>(f::urand(0, 3)));
}

AgentType::~AgentType() {

}

EntityType EntityType::instance;

EntityType& EntityType::getInstance() {
	return instance;
}


EntityType::EntityType() : DataType("entityType") {

}

shared_ptr<DataValue> EntityType::getRandomValue() const {
	return make_shared<EntityTypeValue>(static_cast<EntityTypeEnum>(f::urand(0, 4)));
}

EntityType::~EntityType() {

}

ActivityType ActivityType::instance;

ActivityType& ActivityType::getInstance() {
	return instance;
}


ActivityType::ActivityType() : DataType("activityType") {

}

shared_ptr<DataValue> ActivityType::getRandomValue() const {
	return make_shared<ActivityTypeValue>(static_cast<ActivityTypeEnum>(f::urand(0, 3)));
}

ActivityType::~ActivityType() {

}

} /* namespace datatypes */
} /* namespace fourbench */
