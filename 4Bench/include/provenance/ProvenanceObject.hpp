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

#include "../include/datatypes/DataType.hpp"

using namespace std;

namespace fd = fourbench::datatypes;

namespace fourbench {
namespace provenance {

class ProvenanceObject {
protected:
	unsigned id;
	string domain;
	map<string, shared_ptr<fd::DataValue>> attributes;
	virtual void initialize();

public:
	ProvenanceObject(unsigned id);
	ProvenanceObject(unsigned id, const string& domain);
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
