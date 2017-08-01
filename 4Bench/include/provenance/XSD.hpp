/*
 * XSD.hpp
 *
 *  Created on: Jul 29, 2017
 *      Author: galarraga
 */

#ifndef PROVENANCE_XSD_HPP_
#define PROVENANCE_XSD_HPP_

namespace fourbench {
namespace provenance {

#include <string>

using namespace std;

class XSD {
private:
	XSD();
public:
	static const string prefix;
	static const string integer;
	static const string decimal;
	static const string str;
	static const string date;
	static const string boolean;
	virtual ~XSD();
};

} /* namespace provenance */
} /* namespace fourbench */

#endif /* PROVENANCE_XSD_HPP_ */
