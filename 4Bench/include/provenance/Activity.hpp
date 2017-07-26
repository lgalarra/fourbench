/*
 * Activity.hpp
 *
 *  Created on: Jul 7, 2017
 *      Author: galarraga
 */

#ifndef PROVENANCE_ACTIVITY_HPP_
#define PROVENANCE_ACTIVITY_HPP_

#include <string>

#include "../include/conf/Conf.hpp"
#include "ProvenanceObject.hpp"

using namespace std;

namespace fc = fourbench::conf;

namespace fourbench {
namespace provenance {

class Activity : public ProvenanceObject {
protected:
	void initialize();
public:
	Activity(unsigned id);
	Activity(unsigned id, const string& domain);
	virtual ~Activity();
	string getIRI() const;
	void populateWithAttributes(const fc::ConfValues& conf);
};

} /* namespace provenance */
} /* namespace fourbench */

#endif /* ACTIVITY_HPP_ */
