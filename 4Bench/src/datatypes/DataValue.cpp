/*
 * DataValue.cpp
 *
 *  Created on: Jul 24, 2017
 *      Author: galarraga
 */

#include <time.h>

#include "../include/datatypes/DataValue.hpp"


namespace fourbench {
namespace datatypes {

Date::Date(int day, int month, int year) : day(day), month(month), year(year) {

}

DataValue::DataValue() {
	// TODO Auto-generated constructor stub

}

DataValue::~DataValue() {
	// TODO Auto-generated destructor stub
}

template<typename T> T DataValue::getAs() const {
	void* content = this->get();
	T* contentPointer = static_cast<T*>(content);
	return *contentPointer;
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

IntegerValue::IntegerValue(int n) : DataValue(), intValue(n) {

}

void* IntegerValue::get() const {
	return (void*)&intValue;
}

IntegerValue::~IntegerValue() {

}

BooleanValue::BooleanValue(bool b) : DataValue(), booleanValue(b) {

}

BooleanValue::~BooleanValue() {

}

void* BooleanValue::get() const {
	return (void*)&booleanValue;
}

RatioValue::RatioValue(float f) : DataValue(), floatValue(f) {

}

RatioValue::~RatioValue() {

}

void* RatioValue::get() const {
	return (void*)&floatValue;
}

DateValue::DateValue(time_t d) : DataValue(), dateValue(d) {

}

DateValue::~DateValue() {

}

void* DateValue::get() const {
	return (void*)&dateValue;
}

StringValue::StringValue(const string& d) : DataValue(), stringValue(d) {

}

StringValue::~StringValue() {

}

void* StringValue::get() const {
	return (void*)&stringValue;
}


IRIValue::IRIValue(const string& s) : DataValue(), iriValue(s) {

}

void* IRIValue::get() const {
	return (void*)&iriValue;
}

IRIValue::~IRIValue() {

}

CountryValue::CountryValue(const string& d) : IRIValue(d) {

}

CountryValue::~CountryValue() {

}


AgentTypeValue::AgentTypeValue(AgentTypeEnum arg) : DataValue(), agentType(arg) {

}

void* AgentTypeValue::get() const {
	return (void*)&agentType;
}

AgentTypeValue::~AgentTypeValue() {

}

EntityTypeValue::EntityTypeValue(EntityTypeEnum arg) : DataValue(), entityType(arg) {

}

void* EntityTypeValue::get() const {
	return (void*)&entityType;
}

EntityTypeValue::~EntityTypeValue() {

}

ActivityTypeValue::ActivityTypeValue(ActivityTypeEnum arg) : DataValue(), activityType(arg) {

}

void* ActivityTypeValue::get() const {
	return (void*)&activityType;
}

ActivityTypeValue::~ActivityTypeValue() {

}


} /* namespace datatypes */
} /* namespace fourbench */
