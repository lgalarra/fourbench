/**
 * File: QueryDump.java
 * 
 */
package aau.cs.qweb.fourbench.query.output;

import org.apache.jena.sparql.algebra.Op;

/**
 * Hierarchy of classes that take as input the algebra expressions
 * of an analytical and a provenance query and outputs them in some
 * format.
 * 
 * @author galarraga
 *
 */
public abstract class QuerySerializer {

	public abstract String output(Op analyticalQuery, Op provenanceQuery);
	
}
