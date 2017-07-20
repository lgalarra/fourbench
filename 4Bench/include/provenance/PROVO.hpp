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
};

} /* namespace output */
} /* namespace fourbench */

#endif /* PROVENANCE_PROVO_HPP_ */
