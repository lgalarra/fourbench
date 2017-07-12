/*
 * Triple.hpp
 *
 * It represents an RDF triple
 *
 *  Created on: Jun 30, 2017
 *      Author: galarraga
 */

#include <string>
#include <iostream>

#ifndef PARSING_TRIPLE_HPP_
#define PARSING_TRIPLE_HPP_

using namespace std;

namespace fourbench {
namespace parsing {

class Triple {
private:
	string subject;
	string predicate;
	string object;
	string filename;
	unsigned lineNumber;


public:
	Triple(const string& subject, const string& predicate,
			const string& object, const string& filename, unsigned lineNumber);
	virtual ~Triple();

	string getSubject() const;
	string getPredicate() const;
	string getObject() const;
	unsigned getLineNumber() const;

	friend class FileParser;
};

ostream& operator<<(ostream &strm, const Triple &a);

} /* namespace parsing */
} /* namespace fourbench */

#endif /* PARSING_TRIPLE_HPP_ */
