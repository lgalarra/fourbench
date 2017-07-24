/*
 * DataValueBuilder.hpp
 *
 *  Created on: Jul 24, 2017
 *      Author: galarraga
 */

#ifndef DATATYPES_DATAVALUEBUILDER_HPP_
#define DATATYPES_DATAVALUEBUILDER_HPP_

#include <memory>

using namespace std;

namespace fourbench {
namespace datatypes {

class DataValueBuilder {
private:
	static DataValueBuilder instance;

public:
	DataValueBuilder();
	static DataValueBuilder& getInstance();
	template<class D> shared_ptr<DataValue> get(int arg) {
		return make_shared<D>(arg);
	}
	virtual ~DataValueBuilder();
};

} /* namespace datatypes */
} /* namespace fourbench */

#endif /* DATAVALUEBUILDER_HPP_ */
