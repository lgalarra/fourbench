/*
 * DataValue.hpp
 *
 *  Created on: Jul 24, 2017
 *      Author: galarraga
 */

#ifndef DATATYPES_DATAVALUE_HPP_
#define DATATYPES_DATAVALUE_HPP_

#include <string>

using namespace std;

namespace fourbench {
namespace datatypes {

enum AgentTypeEnum {PERSON, ORGANIZATION, SOFTWARE_AGENT};
enum EntityTypeEnum{FILE, DATABASE, DATA_MAPPING, WEB_SERVICE};
enum ActivityTypeEnum{EXTRACTION, JOIN, AGGREGATION};

class DataValue {
protected:
	virtual void* get() const = 0;
public:
	template<typename T> T getAs() const {
		void* content = this->get();
		T* contentPointer = static_cast<T*>(this->get());
		return *T;
	}

	DataValue();
	virtual ~DataValue();
};

class IntegerValue : public DataValue {
private:
	int intValue;
protected:
	void* get() const;
public:
	IntegerValue(int n);
	virtual ~IntegerValue();
};

class BooleanValue : public DataValue {
private:
	bool booleanValue;
protected:
	void* get() const;
public:
	BooleanValue(bool b);
	virtual ~BooleanValue();
};

class RatioValue : public DataValue {
private:
	float floatValue;
protected:
	void* get() const;
public:
	RatioValue(float f);
	virtual ~RatioValue();
};

typedef struct Date {
	unsigned day;
	unsigned month;
	unsigned year;
};

class DateValue : public DataValue {
private:
	Date dateValue;
protected:
	void* get() const;
public:
	DateValue(const Date& d);
	virtual ~DateValue();
};

class StringValue: public DataValue {
protected:
	string stringValue;
protected:
	void* get() const;
public:
	StringValue(const string& s);
	virtual ~StringValue();
};

class CountryValue: public StringValue {
public:
	CountryValue(const string& s);
	virtual ~CountryValue();
};

class AgentTypeValue : public DataValue {
private:
	AgentTypeEnum agentType;
protected:
	void* get() const;
public:
	AgentTypeValue(AgentTypeEnum arg);
	virtual ~AgentTypeValue();
};

class EntityTypeValue : public DataValue {
private:
	EntityTypeEnum entityType;
protected:
	void* get() const;
public:
	EntityTypeValue(EntityTypeEnum arg);
	virtual ~EntityTypeValue();
};

class ActivityTypeValue : public DataValue {
private:
	ActivityTypeEnum activityType;
protected:
	void* get() const;
public:
	ActivityTypeValue(ActivityTypeEnum arg);
	virtual ~ActivityTypeValue();
};

} /* namespace datatypes */
} /* namespace fourbench */

#endif /* DATATYPES_DATAVALUE_HPP_ */
