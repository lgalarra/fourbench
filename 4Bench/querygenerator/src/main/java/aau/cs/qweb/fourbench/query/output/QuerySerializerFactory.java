/**
 * File: QuerySerializerFactory.java
 * 
 */
package aau.cs.qweb.fourbench.query.output;

/**
 * @author galarraga
 *
 */
public class QuerySerializerFactory {
	
	public static QuerySerializer get(String name) {
		if (name.equals("sparql-graphs")) {
			return new SPARQLGraphQuerySerializer();
		}
		
		return null;
	}

}
