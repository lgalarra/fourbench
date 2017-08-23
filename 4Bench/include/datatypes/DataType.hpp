/*
 * DataType.hpp
 *
 *  Created on: Jul 24, 2017
 *      Author: galarraga
 */

#ifndef DATATYPES_DATATYPE_HPP_
#define DATATYPES_DATATYPE_HPP_

#include <string>
#include <memory>
#include <string>
#include <set>

#include "DataValue.hpp"

using namespace std;

namespace fourbench {
namespace datatypes {

class DataType {
protected:
	string typeName;
	DataType(const string& name);
public:
	string getName() const;
	virtual shared_ptr<DataValue> getRandomValue() const = 0;
	virtual ~DataType();
};

class IntegerType : public DataType {
private:
	static IntegerType instance;
public:
	static IntegerType& getInstance();
	IntegerType();
	shared_ptr<DataValue> getRandomValue() const;
	virtual ~IntegerType();
};

class BooleanType : public DataType {
private:
	static BooleanType instance;
public:
	static BooleanType& getInstance();
	BooleanType();
	shared_ptr<DataValue> getRandomValue() const;
	virtual ~BooleanType();
};

class RatioType : public DataType {
private:
	static RatioType instance;
public:
	static RatioType& getInstance();
	RatioType();
	shared_ptr<DataValue> getRandomValue() const;
	virtual ~RatioType();
};

class DateTimeType : public DataType {
private:
	static DateTimeType instance;
public:
	static DateTimeType& getInstance();
	DateTimeType(const string& s);
	DateTimeType();
	virtual shared_ptr<DataValue> getRandomValue() const;
	virtual ~DateTimeType();

};

class DateType : public DateTimeType {
private:
	static DateType instance;
public:
	static DateType& getInstance();
	DateType();
	shared_ptr<DataValue> getRandomValue() const;
	virtual ~DateType();
};

class EnumeratedDomain {
protected :
	set<string> values;
	EnumeratedDomain(const string& filename);
	string getRandomValue() const;
public :
	virtual ~EnumeratedDomain();
};

class StringType : public DataType, public EnumeratedDomain {
private:
	static StringType instance;
	StringType();
public:
	static StringType& getInstance();
	shared_ptr<DataValue> getRandomValue() const;
	virtual ~StringType();
};

class IRIType : public DataType {
private:
	static IRIType instance;
public:
	static IRIType& getInstance();
	IRIType();
	IRIType(const string& type);
	shared_ptr<DataValue> getRandomValue() const;
	virtual ~IRIType();

};

class CountryType : public IRIType, public EnumeratedDomain {
private:
	static CountryType instance;
public:
	static CountryType& getInstance();
	CountryType();
	shared_ptr<DataValue> getRandomValue() const;
	virtual ~CountryType();
};

class AgentType : public DataType {
private:
	static AgentType instance;
public:
	static AgentType& getInstance();
	AgentType();
	shared_ptr<DataValue> getRandomValue() const;
	virtual ~AgentType();
};

class EntityType : public DataType {
private:
	static EntityType instance;
public:
	static EntityType& getInstance();
	EntityType();
	shared_ptr<DataValue> getRandomValue() const;
	virtual ~EntityType();
};

class PROVOEntityType : public DataType {
private:
	static PROVOEntityType instance;
public:
	static PROVOEntityType& getInstance();
	PROVOEntityType();
	shared_ptr<DataValue> getRandomValue() const;
	virtual ~PROVOEntityType();
};

class ActivityType : public DataType {
private:
	static ActivityType instance;
public:
	static ActivityType& getInstance();
	ActivityType();
	shared_ptr<DataValue> getRandomValue() const;
	virtual ~ActivityType();
};


} /* namespace datatypes */
} /* namespace fourbench */

#endif /* DATATYPES_DATATYPE_HPP_ */
