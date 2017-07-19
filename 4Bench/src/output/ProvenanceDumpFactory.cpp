/*
 * ProvenanceDumpFactory.cpp
 *
 *  Created on: Jul 19, 2017
 *      Author: galarraga
 */

#include "../include/output/ProvenanceDumpFactory.hpp"

namespace fourbench {
namespace output {

ProvenanceDumpFactory ProvenanceDumpFactory::instance;

ProvenanceDumpFactory::ProvenanceDumpFactory() {
	// TODO Auto-generated constructor stub

}

ProvenanceDumpFactory::~ProvenanceDumpFactory() {
	// TODO Auto-generated destructor stub
}

ProvenanceDumpFactory& ProvenanceDumpFactory::getInstance() {
	return instance;
}

} /* namespace output */
} /* namespace fourbench */
