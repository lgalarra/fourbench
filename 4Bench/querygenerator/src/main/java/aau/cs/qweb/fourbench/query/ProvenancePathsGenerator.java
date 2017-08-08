/**
 * File: ProvenancePathsGenerator.java
 * 
 */
package aau.cs.qweb.fourbench.query;

import java.util.List;

import org.apache.jena.graph.Triple;
import org.apache.jena.query.ARQ;
import org.apache.jena.query.Dataset;
import org.apache.jena.sparql.algebra.Algebra;
import org.apache.jena.sparql.algebra.op.OpBGP;
import org.apache.jena.sparql.engine.QueryIterator;
import org.apache.jena.sparql.engine.binding.Binding;

/**
 * @author galarraga
 *
 */
public class ProvenancePathsGenerator {
	
	public static final String UnionGraph = "urn:x-arq:UnionGraph";
	
	/**
	 * @param bgps
	 * @param dataset
	 * @return
	 */
	public static List<List<String>> generate(List<List<Triple>> bgps, Dataset dataset) {
		for (List<Triple> tripleList : bgps) {
			for (Triple t : tripleList) {
				OpBGP singletonBGP = new OpBGP();
				singletonBGP.getPattern().add(t);
				QueryIterator qIter = Algebra.execRef(singletonBGP, dataset.getNamedModel(UnionGraph));
				for ( ; qIter.hasNext() ; )
	            {
	                Binding b = qIter.nextBinding() ;	                
	                System.out.println(b) ; 
	            }
	            qIter.close() ;
			}
		}
		
		return null;
	}

}
