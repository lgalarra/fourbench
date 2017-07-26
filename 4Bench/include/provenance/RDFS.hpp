/*
 * RDFS.hpp
 *
 *  Created on: Jul 25, 2017
 *      Author: galarraga
 */

#ifndef PROVENANCE_RDFS_HPP_
#define PROVENANCE_RDFS_HPP_

namespace fourbench {
namespace provenance {

#include <string>

using namespace std;

class RDFS {
private:
	RDFS();
public:
	static const string prefix;
	static const string label;
	virtual ~RDFS();
};

} /* namespace provenance */
} /* namespace fourbench */

#endif /* RDFS_HPP_ */
