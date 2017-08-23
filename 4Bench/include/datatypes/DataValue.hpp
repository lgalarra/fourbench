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
enum PROVOEntityTypeEnum{COLLECTION, BUNDLE, PLAN};
enum ActivityTypeEnum{EXTRACTION, JOIN, AGGREGATION};

class DataType;

typedef struct Date {
public:
	int day;
	int month;
	int year;

	Date(int day, int month, int year);

} Date;

class DataValue {
protected:
	DataType* type;
	virtual void* get() const = 0;
	DataValue(DataType* type);
public:
	template<typename T> T getAs() const;
	DataType& getType() const;
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

class DateTimeValue : public DataValue {
protected:
	time_t timestamp;
	void* get() const;
	DateTimeValue(time_t arg, DataType* type);
public:
	DateTimeValue(time_t arg);
	virtual ~DateTimeValue();

};

class DateValue : public DateTimeValue {
public:
	DateValue(time_t arg);
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

class IRIValue : public DataValue {
protected:
	string iriValue;
	void* get() const;
	IRIValue(const string& s, DataType* type);
public:
	IRIValue(const string& s);
	virtual ~IRIValue();

};

class CountryValue: public IRIValue {
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

class PROVOEntityTypeValue : public DataValue {
private:
	PROVOEntityTypeEnum entityType;
protected:
	void* get() const;
public:
	PROVOEntityTypeValue(PROVOEntityTypeEnum arg);
	virtual ~PROVOEntityTypeValue();
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
