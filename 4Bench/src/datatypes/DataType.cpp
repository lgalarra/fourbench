/*
 * DataType.cpp
 *
 *  Created on: Jul 24, 2017
 *      Author: galarraga
 */

#include <memory>

#include "../include/datatypes/DataType.hpp"

using namespace std;

namespace fourbench {
namespace datatypes {

DataType::DataType() {

}

DataType::~DataType() {
	// TODO Auto-generated destructor stub
}

string DataType::getName() const {
	return typeName;
}

IntegerType::IntegerType() : DataType(), typeName("integer") {

}

shared_ptr<DataValue> IntegerType::getRandomValue() const {

}

BooleanType::BooleanType() : DataType(), typeName("boolean") {

}

shared_ptr<DataValue> BooleanType::getRandomValue() const {

}

virtual BooleanType::~BooleanType() {

}

RatioType::RatioType() : DataType(), typeName("ratio") {

}

shared_ptr<DataValue> BooleanType::getRandomValue() const {

}

virtual RatioType::~RatioType() {

}

DateType::DateType() : DataType(), typeName("date") {

}

shared_ptr<DataValue> DateType::getRandomValue() const {

}

virtual DateType::~DateType() {

}

StringType::StringType() : DataType(), typeName("string") {

}

shared_ptr<DataValue> StringType::getRandomValue() const {

}

virtual StringType::~StringType() {

}

CountryType::CountryType() : DataType(), typeName("country") {

}

shared_ptr<DataValue> CountryType::getRandomValue() const {

}

CountryType::~CountryType() {

}

AgentType::AgentType() : DataType(), typeName("agentType") {

}

shared_ptr<DataValue> AgentType::getRandomValue() const {

}

virtual AgentType::~AgentType() {

}

EntityType::EntityType() : DataType(), typeName("entityType") {

}

shared_ptr<DataValue> EntityType::getRandomValue() const {

}

virtual EntityType::~EntityType() {

}

ActivityType::ActivityType() : DataType(), typeName("activityType") {

}

shared_ptr<DataValue> ActivityType::getRandomValue() const {

}

virtual ActivityType::~ActivityType() {

}

} /* namespace datatypes */
} /* namespace fourbench */
