/*
 * FileParserFactory.cpp
 *
 *  Created on: Jun 30, 2017
 *      Author: galarraga
 */

#include "../include/parsing/FileParserFactory.hpp"

namespace fourbench {
namespace parsing {

FileParserFactory FileParserFactory::instance;

FileParserFactory::FileParserFactory() {}

FileParserFactory::~FileParserFactory() {

}

FileParserFactory& FileParserFactory::getInstance() {
	return FileParserFactory::instance;
}

} /* namespace parsing */
} /* namespace fourbench */
