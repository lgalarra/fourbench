/*
 * Triple.cpp
 *
 *  Created on: Jun 30, 2017
 *      Author: galarraga
 */


#include <string>

#include "../include/parsing/Triple.hpp"

using namespace std;

namespace fourbench {
namespace parsing {

Triple::Triple(const string& subject, const string& predicate, const string& object, unsigned lineNumber) :
		subject(subject), predicate(predicate), object(object), lineNumber(lineNumber) {}

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

} /* namespace parsing */
} /* namespace fourbench */
