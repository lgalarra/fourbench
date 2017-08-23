/**
 * File: QueryDump.java
 * 
 */
package aau.cs.qweb.fourbench.query.output;

import org.apache.jena.sparql.algebra.Op;

/**
 * @author galarraga
 *
 */
public abstract class QueryOutputter {

	public abstract String output(Op analyticalQuery, Op provenanceQuery);
	
}
