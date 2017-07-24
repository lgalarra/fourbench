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

#include "DataValue.hpp"

using namespace std;

namespace fourbench {
namespace datatypes {

class DataType {
protected:
	string typeName;
	DataType();
public:
	string getName() const;
	virtual shared_ptr<DataValue> getRandomValue() const = 0;
	virtual ~DataType();
};

class IntegerType : public DataType {
public:
	IntegerType();
	shared_ptr<DataValue> getRandomValue() const;
	virtual ~IntegerType();
};

class BooleanType : public DataType {
public:
	BooleanType();
	shared_ptr<DataValue> getRandomValue() const;
	virtual ~BooleanType();
};

class RatioType : public DataType {
public:
	RatioType();
	shared_ptr<DataValue> getRandomValue() const;
	virtual ~RatioType();
};

class DateType : public DataType {
public:
	DateType();
	shared_ptr<DataValue> getRandomValue() const;
	virtual ~DateType();
};

class StringType : public DataType {
public:
	StringType();
	shared_ptr<DataValue> getRandomValue() const;
	virtual ~StringType();
};

class CountryType : public DataType {
public:
	CountryType();
	shared_ptr<DataValue> getRandomValue() const;
	virtual ~CountryType();
};

class AgentType : public DataType {
public:
	AgentType();
	shared_ptr<DataValue> getRandomValue() const;
	virtual ~AgentType();
};

class EntityType : public DataType {
public:
	EntityType();
	shared_ptr<DataValue> getRandomValue() const;
	virtual ~EntityType();
};

class ActivityType : public DataType {
public:
	ActivityType();
	shared_ptr<DataValue> getRandomValue() const;
	virtual ~ActivityType();
};


} /* namespace datatypes */
} /* namespace fourbench */

#endif /* DATATYPES_DATATYPE_HPP_ */
