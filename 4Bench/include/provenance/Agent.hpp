/*
 * Agent.hpp
 *
 *  Created on: Jul 11, 2017
 *      Author: galarraga
 */

#ifndef AGENT_HPP_
#define AGENT_HPP_

#include "ProvenanceObject.hpp"

namespace fourbench {
namespace provenance {

class Agent: public ProvenanceObject {
protected:
	void initialize();
public:
	Agent(unsigned id);
	Agent(unsigned id, const string& domain);
	Agent(unsigned id, const string& domain, unsigned maxNumberOfProperties);
	virtual ~Agent();
	string getIRI() const;
};

} /* namespace provenance */
} /* namespace fourbench */

#endif /* AGENT_HPP_ */
