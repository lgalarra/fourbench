/*
 * Agent.hpp
 *
 *  Created on: Jul 11, 2017
 *      Author: galarraga
 */

#ifndef AGENT_HPP_
#define AGENT_HPP_

#include "../include/conf/Conf.hpp"
#include "ProvenanceObject.hpp"

namespace fc = fourbench::conf;

namespace fourbench {
namespace provenance {

class Agent: public ProvenanceObject {
protected:
	void initialize();
public:
	Agent(unsigned id);
	Agent(unsigned id, const string& domain);
	virtual ~Agent();
	string getIRI() const;
	void populateWithAttributes(const fc::ConfValues& values);
};

} /* namespace provenance */
} /* namespace fourbench */

#endif /* AGENT_HPP_ */
