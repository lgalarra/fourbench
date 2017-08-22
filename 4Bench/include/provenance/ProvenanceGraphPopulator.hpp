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

#include "../include/provenance/PopulateStats.hpp"
#include "../include/parsing/FileParser.hpp"
#include "../include/output/ProvenanceDump.hpp"


namespace fpa = fourbench::parsing;
namespace fo = fourbench::output;

namespace fourbench {
namespace provenance {

class ProvenanceGraphPopulator {
private:
	shared_ptr<fo::ProvenanceDump> output;

public:
	ProvenanceGraphPopulator(shared_ptr<fo::ProvenanceDump> out);

	shared_ptr<map<string, shared_ptr<PopulateStats>>> populate(shared_ptr<fpa::FileParser> parser,
			shared_ptr<map<string, shared_ptr<ProvenanceGraph>>> graphs);

	virtual ~ProvenanceGraphPopulator();
};

} /* namespace provenance */
} /* namespace fourbench */

#endif /* PROVENANCEGRAPHPOPULATOR_HPP_ */
