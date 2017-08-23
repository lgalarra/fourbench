/*
 * PROVO.hpp
 *
 *  Created on: Jul 20, 2017
 *      Author: galarraga
 */

#ifndef PROVENANCE_PROVO_HPP_
#define PROVENANCE_PROVO_HPP_

#include <string>

using namespace std;

namespace fourbench {
namespace provenance {

class PROVO {
private:
	PROVO();
public:
	virtual ~PROVO();
	static const string prefix;
	static const string wasAttributedTo;
	static const string wasAssociatedWith;
	static const string used;
	static const string wasGeneratedBy;
	static const string startedAtTime;
	static const string endedAtTime;
	static const string Activity;
	static const string Agent;
	static const string Entity;
	static const string Person;
	static const string Organization;
	static const string SoftwareAgent;
	static const string Collection;
	static const string Bundle;
	static const string Plan;
};

} /* namespace output */
} /* namespace fourbench */

#endif /* PROVENANCE_PROVO_HPP_ */
