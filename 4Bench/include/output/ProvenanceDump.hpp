/*
 * ProvenanceDump.hpp
 *
 *  Created on: Jul 19, 2017
 *      Author: galarraga
 */

#ifndef OUTPUT_PROVENANCEDUMP_HPP_
#define OUTPUT_PROVENANCEDUMP_HPP_

#include <ostream>
#include <vector>

#include "../include/parsing/Triple.hpp"
#include "../include/provenance/ProvenanceObject.hpp"
#include "../include/provenance/Entity.hpp"
#include "../include/provenance/ProvenanceGraph.hpp"

namespace fpar = fourbench::parsing;
namespace fprov = fourbench::provenance;
namespace fd = fourbench::datatypes;

using namespace std;

namespace fourbench {
namespace output {

class ProvenanceDump {
protected:
	ostream &stream;
	long provenanceTriplesDumped;
	long triplesDumped;

	ProvenanceDump(ostream& strm);
	virtual ostream& formatIRI(const string& iri) const;
	virtual ostream& format(shared_ptr<fd::DataValue> value) const;
	virtual ostream& formatInteger(int integer) const;
	virtual ostream& formatBoolean(bool b) const;
	virtual ostream& formatFloat(float decimal) const;
	virtual ostream& formatCountry(const string& str) const;
	virtual ostream& formatString(const string& str) const;
	virtual ostream& formatDate(const fd::Date& date) const;
	virtual ostream& formatDateTime(time_t tstamp) const;
	virtual void dump(const fprov::ProvenanceObject& obj, string attributeName, shared_ptr<fd::DataValue> attributeValue) = 0;
	virtual void dump(const fprov::ProvenanceObject& obj);
	virtual ostream& formatActivityType(fd::ActivityTypeEnum type) const;
	virtual ostream& formatEntityType(fd::EntityTypeEnum type) const;
	virtual ostream& formatAgentType(fd::AgentTypeEnum type) const;
public:
	virtual void dump(const fpar::Triple& triple, const fprov::Entity& provEntity) = 0;
	// TODO: Change the signature to receive a constant graph
	virtual void dump(shared_ptr<fprov::ProvenanceGraph> graph) = 0;
	long getNumberOfDumpedProvenanceTriples() const;
	long getNumberOfDumpedTriples() const;
	void flush();
	virtual ~ProvenanceDump();

	friend class ProvenanceDumpFactory;
};

} /* namespace output */
} /* namespace fourbench */

#endif /* OUTPUT_PROVENANCEDUMP_HPP_ */
