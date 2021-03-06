/*
 * Entity.hpp
 *
 *  Created on: Jul 11, 2017
 *      Author: galarraga
 */

#ifndef PROVENANCE_ENTITY_HPP_
#define PROVENANCE_ENTITY_HPP_


#include "ProvenanceObject.hpp"
#include "EntityLevel.hpp"

namespace fourbench {
namespace provenance {

class Entity : public ProvenanceObject {
private:
	EntityLevel level;
protected:
	void initialize();
public:
	Entity(unsigned id);
	Entity(unsigned id, const string& domain);
	Entity(unsigned id, const string& domain, unsigned maxNumberOfProperties);
	Entity(unsigned id, const string& domain, unsigned maxNumberOfProperties, EntityLevel level);
	virtual ~Entity();
	string getIRI() const;

};

} /* namespace provenance */
} /* namespace fourbench */

#endif /* ENTITY_HPP_ */
