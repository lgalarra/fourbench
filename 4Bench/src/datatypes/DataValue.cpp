/*
 * DataValue.cpp
 *
 *  Created on: Jul 24, 2017
 *      Author: galarraga
 */

#include "../include/datatypes/DataValue.hpp"

namespace fourbench {
namespace datatypes {

DataValue::DataValue() {
	// TODO Auto-generated constructor stub

}

DataValue::~DataValue() {
	// TODO Auto-generated destructor stub
}

IntegerValue::IntegerValue(int n) : DataValue(), intValue(n) {

}

void* IntegerValue::get() const {
	return (void*)&intValue;
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

DateValue::DateValue(const Date& d) : DataValue(), dateValue(d) {

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

CountryValue::CountryValue(const string& d) : StringValue(d) {

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
