/*
 * Agent.hpp
 *
 *  Created on: Jul 11, 2017
 *      Author: galarraga
 */

#ifndef AGENT_HPP_
#define AGENT_HPP_

#include <provenance/ProvenanceObject.hpp>

namespace fourbench {
namespace provenance {

class Agent: public ProvenanceObject {
public:
	Agent(unsigned id);
	Agent(unsigned id, const string& domain);
	virtual ~Agent();
	string getIRI() const;
};

} /* namespace provenance */
} /* namespace fourbench */

#endif /* AGENT_HPP_ */
