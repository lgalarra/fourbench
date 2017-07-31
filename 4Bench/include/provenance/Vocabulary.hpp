/*
 * Vocabulary.hpp
 *
 *  Created on: Jul 28, 2017
 *      Author: galarraga
 */

#ifndef PROVENANCE_VOCABULARY_HPP_
#define PROVENANCE_VOCABULARY_HPP_

namespace fourbench {
namespace provenance {

#include <string>

using namespace std;

class Vocabulary {
private:
	Vocabulary();
public:
	static const string sourceType;
	static const string agentType;
	static const string id;
	static const string SoftwareAgent;
	virtual ~Vocabulary();
};

} /* namespace provenance */
} /* namespace fourbench */

#endif /* VOCABULARY_HPP_ */
