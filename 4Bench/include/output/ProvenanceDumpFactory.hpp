/*
 * ProvenanceDumpFactory.hpp
 *
 *  Created on: Jul 19, 2017
 *      Author: galarraga
 */

#ifndef OUTPUT_PROVENANCEDUMPFACTORY_HPP_
#define OUTPUT_PROVENANCEDUMPFACTORY_HPP_

#include <memory>
#include <string>
#include <ostream>

using namespace std;

namespace fourbench {
namespace output {

class ProvenanceDumpFactory {

private:
	static ProvenanceDumpFactory instance;
	ProvenanceDumpFactory();

public:
	static ProvenanceDumpFactory& getInstance();

	template <class Dump> shared_ptr<Dump> buildDump(ostream& strm) {
		return make_shared<Dump>(strm);
	}

	virtual ~ProvenanceDumpFactory();
};

} /* namespace output */
} /* namespace fourbench */

#endif /* OUTPUT_PROVENANCEDUMPFACTORY_HPP_ */
