/**
 * File: OpWalker.java
 * 
 */
package aau.cs.qweb.fourbench.query;

import java.util.ArrayList;
import java.util.Collection;
import java.util.List;

import org.apache.jena.graph.Triple;
import org.apache.jena.sparql.algebra.Op;
import org.apache.jena.sparql.algebra.op.OpBGP;
import org.apache.jena.sparql.algebra.op.OpJoin;
import org.apache.jena.sparql.algebra.op.OpProject;
import org.apache.jena.sparql.algebra.op.OpUnion;

/**
 * @author galarraga
 *
 */
public class ImprovedOpWalker {

	public static List<List<Triple>> walk(Op op) {
		List<List<OpBGP>> partialResult = new ArrayList<>();		
		List<List<Triple>> result = new ArrayList<>();		
		
		traverse(op, partialResult);
		
		for (List<OpBGP> lbgp : partialResult) {
			List<Triple> triples = new ArrayList<>();
			for (OpBGP bgp : lbgp) {
				triples.addAll(bgp.getPattern().getList());
			}
			
			result.add(triples);
		}
		
		return result;
		
	}

	/**
	 * @param op
	 * @param result
	 */
	private static void traverse(Op op, List<List<OpBGP>> result) {
		if (op instanceof OpProject) {
			OpProject opProject = (OpProject) op;
			traverse(opProject.getSubOp(), result);
		} else if (op instanceof OpJoin) {
			OpJoin opJoin = (OpJoin) op;
			List<List<OpBGP>> partialResultLeft = new ArrayList<>();
			List<List<OpBGP>> partialResultRight = new ArrayList<>();
			traverse(opJoin.getLeft(), partialResultLeft);
			traverse(opJoin.getRight(), partialResultRight);
			result.addAll(combine(partialResultLeft, partialResultRight));
		} else if (op instanceof OpBGP) {
			List<OpBGP> singleton = new ArrayList<>();
			singleton.add((OpBGP)op);
			result.add(singleton);
		} else if (op instanceof OpUnion) {
			OpUnion opUnion = (OpUnion) op;
			traverse(opUnion.getLeft(), result);
			traverse(opUnion.getRight(), result);
			
		}
	}

	/**
	 * @param partialResultLeft
	 * @param partialResultRight
	 * @return
	 */
	private static Collection<? extends List<OpBGP>> combine(List<List<OpBGP>> left,
			List<List<OpBGP>> right) {
		List<List<OpBGP>> result = new ArrayList<>();
		
		for (List<OpBGP> l : left) {
			for (List<OpBGP> r : right) {
				List<OpBGP> lr = new ArrayList<>();
				lr.addAll(l);
				lr.addAll(r);
				result.add(lr);
			}
		}
		
		return result;
	}

}
