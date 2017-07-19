/*
 * QuadTSVProvenanceDump.hpp
 *
 *  Created on: Jul 19, 2017
 *      Author: galarraga
 */

#ifndef OUTPUT_QUADTSVPROVENANCEDUMP_HPP_
#define OUTPUT_QUADTSVPROVENANCEDUMP_HPP_

#include "../include/output/ProvenanceDump.hpp"

namespace fpar = fourbench::parsing;
namespace fprov = fourbench::provenance;

using namespace std;

namespace fourbench {
namespace output {

class QuadTSVProvenanceDump: public ProvenanceDump {
public:
	QuadTSVProvenanceDump(ostream& strm);
	void dump(const fpar::Triple& triple, const fprov::Entity& provEntity) const;
	void dump(fprov::ProvenanceGraph& graph) const;
	virtual ~QuadTSVProvenanceDump();
};

} /* namespace output */
} /* namespace fourbench */

#endif /* QUADTSVPROVENANCEDUMP_HPP_ */
