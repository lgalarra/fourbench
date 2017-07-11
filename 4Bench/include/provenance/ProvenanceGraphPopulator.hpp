/*
 * ProvenanceGraphPopulator.hpp
 *
 *  Created on: Jul 11, 2017
 *      Author: galarraga
 */

#ifndef PROVENANCE_PROVENANCEGRAPHPOPULATOR_HPP_
#define PROVENANCE_PROVENANCEGRAPHPOPULATOR_HPP_

#include <algorithm>
#include <memory>
#include <string>

#include "../include/parsing/FileParser.hpp"

namespace fpa = fourbench::parsing;

namespace fourbench {
namespace provenance {

class ProvenanceGraphPopulator {
public:
	ProvenanceGraphPopulator();

	void populate(fpa::FileParser& parser, map<string, shared_ptr<ProvenanceGraph>>& graphs);

	virtual ~ProvenanceGraphPopulator();
};

} /* namespace provenance */
} /* namespace fourbench */

#endif /* PROVENANCEGRAPHPOPULATOR_HPP_ */
