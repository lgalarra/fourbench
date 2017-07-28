/*
 * ProvenanceObject.hpp
 *
 * Parent class of all PROV-O concepts such as provenance entities, activities and agents.
 *  Created on: Jul 7, 2017
 *      Author: galarraga
 */

#ifndef PROVENANCE_PROVENANCEOBJECT_HPP_
#define PROVENANCE_PROVENANCEOBJECT_HPP_

#include <string>
#include <ostream>
#include <map>
#include <memory>
#include <utility>

#include "../include/datatypes/DataType.hpp"
#include "../include/datatypes/DataValue.hpp"

using namespace std;

namespace fd = fourbench::datatypes;

namespace fourbench {
namespace provenance {

class ProvenanceObject {
protected:
	unsigned id;
	string domain;
	unsigned maxNumberOfAttributes;
	map<string, shared_ptr<fd::DataValue>> attributes;
	virtual void initialize();
	static const map<string, fd::DataType*> attributeTypes;
public:
	ProvenanceObject(unsigned id);
	ProvenanceObject(unsigned id, const string& domain);
	ProvenanceObject(unsigned id, const string& domain, unsigned maxNumberOfAttributes);
	pair<map<string, shared_ptr<fd::DataValue>>::const_iterator, map<string, shared_ptr<fd::DataValue>>::const_iterator> getAttributeIterators() const;
	virtual ~ProvenanceObject();
	unsigned getId() const;
	virtual string getIRI() const = 0;
	ostream& dump(ostream& o) const;
	ostream& operator<<(ostream& ostrm) {
		return dump(ostrm);
	}
};


} /* namespace provenance */
} /* namespace fourbench */

#endif /* PROVENANCE_PROVENANCEOBJECT_HPP_ */
