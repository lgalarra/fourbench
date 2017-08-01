/*
 * DataValue.cpp
 *
 *  Created on: Jul 24, 2017
 *      Author: galarraga
 */

#include <time.h>

#include "../include/datatypes/DataValue.hpp"
#include "../include/datatypes/DataType.hpp"


namespace fourbench {
namespace datatypes {

Date::Date(int day, int month, int year) : day(day), month(month), year(year) {

}

DataValue::DataValue(DataType* type) : type(type) {

}

DataValue::~DataValue() {
	// TODO Auto-generated destructor stub
}

template<typename T> T DataValue::getAs() const {
	void* content = this->get();
	T* contentPointer = static_cast<T*>(content);
	return *contentPointer;
}

DataType& DataValue::getType() const {
	return *type;
}

template<> Date DataValue::getAs<Date>() const {
	void* content = this->get();
	time_t* timeValPoint = static_cast<time_t*>(content);
	char day[3], month[3], year[5];
	struct tm *ts = localtime(timeValPoint);
	strftime(day, sizeof(day), "%d", ts);
	strftime(month, sizeof(month), "%m", ts);
	strftime(year, sizeof(year), "%Y", ts);
	Date d(atoi(day), atoi(month), atoi(year));
	return d;
}

template AgentTypeEnum DataValue::getAs<AgentTypeEnum>() const;
template string DataValue::getAs<string>() const;
template int DataValue::getAs<int>() const;
template bool DataValue::getAs<bool>() const;
template float DataValue::getAs<float>() const;
template ActivityTypeEnum DataValue::getAs<ActivityTypeEnum>() const;
template EntityTypeEnum DataValue::getAs<EntityTypeEnum>() const;

IntegerValue::IntegerValue(int n) : DataValue(&IntegerType::getInstance()), intValue(n) {

}

void* IntegerValue::get() const {
	return (void*)&intValue;
}

IntegerValue::~IntegerValue() {

}

BooleanValue::BooleanValue(bool b) : DataValue(&BooleanType::getInstance()), booleanValue(b) {

}

BooleanValue::~BooleanValue() {

}

void* BooleanValue::get() const {
	return (void*)&booleanValue;
}

RatioValue::RatioValue(float f) : DataValue(&RatioType::getInstance()), floatValue(f) {

}

RatioValue::~RatioValue() {

}

void* RatioValue::get() const {
	return (void*)&floatValue;
}

DateValue::DateValue(time_t d) : DataValue(&DateType::getInstance()), dateValue(d) {

}

DateValue::~DateValue() {

}

void* DateValue::get() const {
	return (void*)&dateValue;
}

StringValue::StringValue(const string& d) : DataValue(&StringType::getInstance()), stringValue(d) {

}

StringValue::~StringValue() {

}

void* StringValue::get() const {
	return (void*)&stringValue;
}


IRIValue::IRIValue(const string& s, DataType* type) : DataValue(type), iriValue(s) {

}

IRIValue::IRIValue(const string& s) : DataValue(&IRIType::getInstance()), iriValue(s) {

}

void* IRIValue::get() const {
	return (void*)&iriValue;
}

IRIValue::~IRIValue() {

}

CountryValue::CountryValue(const string& d) : IRIValue(d, &CountryType::getInstance()) {

}

CountryValue::~CountryValue() {

}


AgentTypeValue::AgentTypeValue(AgentTypeEnum arg) : DataValue(&AgentType::getInstance()), agentType(arg) {

}

void* AgentTypeValue::get() const {
	return (void*)&agentType;
}

AgentTypeValue::~AgentTypeValue() {

}

EntityTypeValue::EntityTypeValue(EntityTypeEnum arg) : DataValue(&EntityType::getInstance()), entityType(arg) {

}

void* EntityTypeValue::get() const {
	return (void*)&entityType;
}

EntityTypeValue::~EntityTypeValue() {

}

ActivityTypeValue::ActivityTypeValue(ActivityTypeEnum arg) : DataValue(&ActivityType::getInstance()), activityType(arg) {

}

void* ActivityTypeValue::get() const {
	return (void*)&activityType;
}

ActivityTypeValue::~ActivityTypeValue() {

}


} /* namespace datatypes */
} /* namespace fourbench */
