/*
 * Triple.cpp
 *
 *  Created on: Jun 30, 2017
 *      Author: galarraga
 */


#include <string>
#include <iostream>

#include "../include/parsing/Triple.hpp"

using namespace std;

namespace fourbench {
namespace parsing {

Triple::Triple(const string& subject, const string& predicate, const string& object,
		const string& filename, unsigned lineNumber) :
		subject(subject), predicate(predicate), object(object),
		filename(filename), lineNumber(lineNumber) {}

Triple::~Triple() {}

string Triple::getSubject() const {
	return subject;
}
string Triple::getPredicate() const {
	return predicate;
}

string Triple::getObject() const {
	return object;
}

unsigned Triple::getLineNumber() const {
	return lineNumber;
}

ostream& operator<<(ostream &strm, const Triple &a) {
	strm << "[" << a.getSubject() << ", " << a.getPredicate() << ", " << a.getObject() << "]";
	return strm;
}

} /* namespace parsing */
} /* namespace fourbench */
