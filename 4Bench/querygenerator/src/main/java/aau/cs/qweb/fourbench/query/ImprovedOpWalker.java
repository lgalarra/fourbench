/**
 * File: OpWalker.java
 * 
 */
package aau.cs.qweb.fourbench.query;

import java.util.ArrayList;
import java.util.Collection;
import java.util.LinkedHashMap;
import java.util.LinkedHashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;

import org.apache.jena.graph.Node;
import org.apache.jena.graph.Triple;
import org.apache.jena.sparql.algebra.Op;
import org.apache.jena.sparql.algebra.op.Op1;
import org.apache.jena.sparql.algebra.op.OpBGP;
import org.apache.jena.sparql.algebra.op.OpJoin;
import org.apache.jena.sparql.algebra.op.OpProject;
import org.apache.jena.sparql.algebra.op.OpUnion;
import org.apache.jena.sparql.core.Var;

/**
 * @author galarraga
 *
 */
public class ImprovedOpWalker {

	public static List<Set<Triple>> walk(Op op) {
		List<List<OpBGP>> partialResult = new ArrayList<>();		
		List<Set<Triple>> result = new ArrayList<>();		
		
		traverse(op, partialResult);
		
		for (List<OpBGP> lbgp : partialResult) {
			Set<Triple> triples = new LinkedHashSet<>();
			for (OpBGP bgp : lbgp) {
				triples.addAll(normalize(bgp.getPattern().getList()));
			}
			
			result.add(triples);
		}
		
		return result;
		
	}

	/**
	 * It guarantees that the list of triple patterns is given as a connected
	 * sequence of triple patterns.
	 * @param list
	 * @return
	 */
	private static Collection<? extends Triple> normalize(List<Triple> list) {
		if (list.size() < 3)
			return list;
			
		List<Triple> result = new ArrayList<>();
		Map<Var, List<Triple>> var2Triple = new LinkedHashMap<>();
		Map<Triple, List<Var>> triple2Var = new LinkedHashMap<>();
		
		for (Triple t : list) {
			Node subject = t.getSubject();
			Node predicate = t.getPredicate();
			Node object = t.getObject();
			
			if (subject.isVariable()) {
				Var subVar = Var.alloc(subject);
				ImprovedOpWalker.put(subVar, t, var2Triple);
				ImprovedOpWalker.put(t, subVar, triple2Var);
			}
			
			if (predicate.isVariable()) {
				Var subPred = Var.alloc(predicate);
				ImprovedOpWalker.put(subPred, t, var2Triple);
				ImprovedOpWalker.put(t, subPred, triple2Var);
			}
			
			if (object.isVariable()) {
				Var obVar = Var.alloc(object);
				ImprovedOpWalker.put(obVar, t, var2Triple);
				ImprovedOpWalker.put(t, obVar, triple2Var);
			}
		}
		
		Set<Var> visitedVars = new LinkedHashSet<>();
		Set<Triple> visitedTriples = new LinkedHashSet<>();
		Triple seed = triple2Var.keySet().iterator().next();
		Var seedVariable = null;
		while (result.size() < list.size()) {
			result.add(seed);
			visitedTriples.add(seed);
			List<Var> vars = triple2Var.get(seed);
			for (Var v : vars) {
				if (!visitedVars.contains(v)) {
					visitedVars.add(v);
					seedVariable = v;
					break;
				}
					
			}
			
			for (Triple t : var2Triple.get(seedVariable)) {
				if (!visitedTriples.contains(t)) {
					seed = t;
					break;
				}
			}
		}
		
		return result;
	}
	
	private static <T, U> void put(T t, U u, Map<T, List<U>> aMap) {
		List<U> values = aMap.get(t);
		if (values == null) {
			values = new ArrayList<>();
			aMap.put(t, values);
		}
		values.add(u);
	}

	/**
	 * @param op
	 * @param result
	 */
	private static void traverse(Op op, List<List<OpBGP>> result) {
		if (op instanceof Op1) {
			Op1 opUnary = (Op1) op;
			traverse(opUnary.getSubOp(), result);
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
