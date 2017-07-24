/*
 * RDF.hpp
 *
 *  Created on: Jul 24, 2017
 *      Author: galarraga
 */

#ifndef PROVENANCE_RDF_HPP_
#define PROVENANCE_RDF_HPP_

#include <string>

using namespace std;

namespace fourbench {
namespace provenance {

class RDF {
private:
	RDF();
public:
	virtual ~RDF();
	static const string type;
};

} /* namespace provenance */
} /* namespace fourbench */

#endif /* RDF_HPP_ */
