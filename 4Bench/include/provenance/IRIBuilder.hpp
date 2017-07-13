/*
 * IRIBuilder.hpp
 *
 *  Created on: Jul 11, 2017
 *      Author: galarraga
 */

#ifndef PROVENANCE_IRIBUILDER_HPP_
#define PROVENANCE_IRIBUILDER_HPP_

#include <string>

#include "IRIType.hpp"

using namespace std;

namespace fourbench {
namespace provenance {

class IRIBuilder {
private:
	static IRIBuilder instance;
	static const string defaultDomain;
	IRIBuilder();

public:
	static IRIBuilder& getInstance();
	static string getDefaultDomain();

	string getIRI(IRIType type, unsigned id) const;
	string getIRI(const string& domain, IRIType type, unsigned id) const;
	virtual ~IRIBuilder();
};

} /* namespace provenance */
} /* namespace fourbench */

#endif /* IRIBUILDER_HPP_ */
