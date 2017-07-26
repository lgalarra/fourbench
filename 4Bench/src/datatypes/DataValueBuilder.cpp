/*
 * DataValueBuilder.cpp
 *
 *  Created on: Jul 24, 2017
 *      Author: galarraga
 */

#include "../include/datatypes/DataValueBuilder.hpp"

namespace fourbench {
namespace datatypes {

DataValueBuilder DataValueBuilder::instance;

DataValueBuilder::DataValueBuilder() {

}

DataValueBuilder::~DataValueBuilder() {
	// TODO Auto-generated destructor stub
}

DataValueBuilder& DataValueBuilder::getInstance() {
	return instance;
}

} /* namespace datatypes */
} /* namespace fourbench */
