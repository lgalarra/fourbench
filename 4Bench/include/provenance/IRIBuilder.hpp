/*
 * IRIBuilder.hpp
 *
 *  Created on: Jul 11, 2017
 *      Author: galarraga
 */

#ifndef PROVENANCE_IRIBUILDER_HPP_
#define PROVENANCE_IRIBUILDER_HPP_

#include <string>
#include <map>
#include <memory>

#include "IRIType.hpp"

using namespace std;

namespace fourbench {
namespace provenance {

class IRIBuilder {
private:
	static const string defaultDomain;
	string domain;
	IRIBuilder(const string& prefix);
public:
	static map<string, shared_ptr<IRIBuilder>> instances;
	static shared_ptr<IRIBuilder> getInstance(const string& prefix);
	static string getDefaultDomain();

	string getIRI(IRIType type, unsigned id) const;
	string getDomain() const;
	virtual ~IRIBuilder();
};

} /* namespace provenance */
} /* namespace fourbench */

#endif /* IRIBUILDER_HPP_ */
