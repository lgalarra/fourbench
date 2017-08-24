/**
 * File: SPARQLGraphQueryOutputter.java
 * 
 */
package aau.cs.qweb.fourbench.query.output;


import org.apache.jena.graph.Node;
import org.apache.jena.graph.NodeFactory;
import org.apache.jena.query.Query;
import org.apache.jena.sparql.algebra.Op;
import org.apache.jena.sparql.algebra.OpAsQuery;
import org.apache.jena.sparql.algebra.op.Op1;
import org.apache.jena.sparql.algebra.op.OpGraph;
import org.apache.jena.sparql.algebra.op.OpJoin;
import org.apache.jena.sparql.algebra.op.OpProject;
import org.apache.jena.sparql.core.Var;

import aau.cs.qweb.fourbench.query.Config;

/**
 * @author galarraga
 *
 */
public class SPARQLGraphQuerySerializer extends QuerySerializer {

	/* (non-Javadoc)
	 * @see aau.cs.qweb.fourbench.query.output.QueryOutputter#output(org.apache.jena.sparql.algebra.Op, org.apache.jena.sparql.algebra.Op)
	 */
	@Override
	public String output(Op analyticalQuery, Op provenanceQuery) {		
		// Surround the provenance graph around a graph <provenanceGraph> {} block
		OpProject provenanceProjection = getProjection(provenanceQuery);
		if (provenanceProjection == null) {
			System.err.println("The program could not find the project in the provenance query.");
			return null;
		}
		
		Node provenanceGraphURI = NodeFactory.createURI(Config.provenanceGraphURI);
		OpGraph provenanceGraph = new OpGraph(provenanceGraphURI, provenanceQuery);
		
		// Surround the first non-op1 operator in the analytical query by a graph ?g {} block
		Var provenanceProjVar = provenanceProjection.getVars().get(0);
		Op inflexionPoint = getFirstNonUnaryOperator(analyticalQuery);		
		OpGraph analyticalGraph = new OpGraph(provenanceProjVar, inflexionPoint);
		
		// We have to join the graphs 
		OpJoin join = (OpJoin) OpJoin.create(provenanceGraph, analyticalGraph);
		// First iterate until we get to the projection of the analytical query
		Op result = paste(analyticalQuery, inflexionPoint, join);
		Query resultQuery = OpAsQuery.asQuery(result);
		return resultQuery.toString();
		
	}

	/**
	 * @param root
	 * @param inflexionPoint 
	 * @param tail
	 * @return
	 */
	private Op paste(Op root, Op inflexionPoint, OpJoin tail) {
		// Search until inflection point
		Op1 parent = (Op1) inflexionPoint;
		Op currentOp = tail;
		
		while (parent != null) {
			Op copyParent = parent.copy(currentOp);
			parent = findParent(root, parent);
			currentOp = copyParent;
		}
		
		return currentOp;
	}

	/**
	 * @param node
	 * @return
	 */
	private Op1 findParent(Op root, Op1 node) {
		Op cursor = node;
		
		if (node == root)
			return null;
		
		while (cursor instanceof Op1) {
			if (((Op1)cursor).getSubOp() == node) {
				return (Op1)cursor;
			}
			cursor = ((Op1)cursor).getSubOp();
		}
		
		return null;
	}

	/**
	 * @param analyticalQuery
	 * @return
	 */
	private Op getFirstNonUnaryOperator(Op analyticalQuery) {
		Op currentOp = analyticalQuery;
		while (currentOp instanceof Op1) {
			currentOp = ((Op1) currentOp).getSubOp();
		}
		
		return currentOp;
	}

	/**
	 * @param analyticalQuery
	 * @return
	 */
	private OpProject getProjection(Op analyticalQuery) {
		Op currentOperator = analyticalQuery;
		while (!(currentOperator instanceof OpProject)) {
			if (!(currentOperator instanceof Op1)) {
				return null;
			} else {
				Op1 op = (Op1) currentOperator;
				currentOperator = op.getSubOp();
			}
			
		}
		
		return (OpProject) currentOperator;
	}

}
