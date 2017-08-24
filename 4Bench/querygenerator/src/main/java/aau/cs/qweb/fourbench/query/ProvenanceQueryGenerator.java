/**
 * File: ProvenanceQueryGenerator.java
 * 
 */
package aau.cs.qweb.fourbench.query;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.LinkedHashSet;
import java.util.List;
import java.util.Set;
import java.util.Stack;

import org.apache.commons.lang3.tuple.MutablePair;
import org.apache.commons.lang3.tuple.Pair;
import org.apache.jena.graph.Node;
import org.apache.jena.graph.NodeFactory;
import org.apache.jena.graph.Triple;
import org.apache.jena.query.Dataset;
import org.apache.jena.rdf.model.Model;
import org.apache.jena.sparql.algebra.Algebra;
import org.apache.jena.sparql.algebra.Op;
import org.apache.jena.sparql.algebra.op.OpBGP;
import org.apache.jena.sparql.algebra.op.OpProject;
import org.apache.jena.sparql.core.BasicPattern;
import org.apache.jena.sparql.core.Var;
import org.apache.jena.sparql.engine.QueryIterator;
import org.apache.jena.sparql.engine.binding.Binding;

/**
 * @author galarraga
 *
 */
public class ProvenanceQueryGenerator {
	
	public static final int MAX_QUERY_SIZE = 15;
	
	Dataset dataset;
	
	Model provenanceGraph;
	
	/**
	 * @param dataset
	 */
	public ProvenanceQueryGenerator(Dataset dataset) {
		this.dataset = dataset;
		this.provenanceGraph = dataset.getNamedModel(Config.provenanceGraphURI);
	}
	
	Triple computeBGPIteration(Stack<Triple> triplesStack, boolean subject2Object, int level, 
			Triple currentTriple, BasicPattern basicPattern, Set<String> flattenedSet) {
		Var var1 = Var.alloc("p");
		Var var2 = Var.alloc("o");
		Set<String> currentSet = null;
		
		if (flattenedSet != null) {
			currentSet = flattenedSet;
		} else {
			currentSet = getBindingsSet(basicPattern, currentTriple);
		}
		
		boolean firstSetOfBindings = true;			
		Set<Pair<Node, Node>> predicateValues = new LinkedHashSet<>();
		Set<Node> predicates = new LinkedHashSet<>();
		
		for (String binding : currentSet) {				
			Triple boundPattern = null;
			BasicPattern bp = new BasicPattern();
			if (level == 0) {
				boundPattern = new Triple(NodeFactory.createURI(binding), var1, var2);
			} else {
				boundPattern = bindTriple(currentTriple, NodeFactory.createURI(binding));
				Triple nextPattern = new Triple(NodeFactory.createURI(binding), var1, var2);
				bp.add(nextPattern);
			}
			
			bp.addAll(basicPattern);
			bp.add(boundPattern);
			Op op = new OpProject(new OpBGP(bp), Arrays.asList(var1, var2));
			QueryIterator qIter = Algebra.exec(op, provenanceGraph) ;
			if (firstSetOfBindings) {
				firstSetOfBindings = false;
				addBindings(binding, var1, var2, qIter, predicateValues, predicates);
			} else {
				intersectBindings(binding, var1, var2, qIter, predicateValues, predicates);
			}
			qIter.close();
			
			// No reason to continue
			if (predicates.isEmpty() && predicateValues.isEmpty()) {
				break;
			}
		}
		
		if (currentTriple != null)
			basicPattern.add(currentTriple);

		Var subjectVar = currentTriple == null ? Var.alloc("i" + level) : getDanglingVariable(currentTriple);
		Set<Node> excludedProperties = addBoundTriplePatterns(currentTriple, subjectVar, predicateValues, level, basicPattern);
		
		// And try to specialize the remaining properties
		predicates.removeAll(excludedProperties);
		scheduleNewPaths(predicates, subjectVar, level, triplesStack);
		
		if (triplesStack.isEmpty())
			return null;
		
		return triplesStack.pop();
	}

	/**
	 * @param startBindingsSet
	 * @return
	 */
	public OpBGP computeBGP(Set<String> startBindingsSet) {	
		BasicPattern basicPattern = new BasicPattern();
		int level = 0;
		Stack<Triple> triplesStack = new Stack<>();
		Triple currentTriple = null;
		
		do {
			if (level == 0) {
				currentTriple = computeBGPIteration(triplesStack, true, 
						level, currentTriple, basicPattern, startBindingsSet);		
			} else {
				currentTriple = computeBGPIteration(triplesStack, true, 
						level, currentTriple, basicPattern, null);	
			}
			++level;
		} while (currentTriple != null);
		
		return new OpBGP(basicPattern);
	}

	/**
	 * @param predicates
	 * @param subjectVar
	 * @param level
	 * @param triplesStack
	 */
	private void scheduleNewPaths(Set<Node> predicates, Var subjectVar, int level, Stack<Triple> triplesStack) {
		int count = 0;			
		for (Node predicate : predicates) {
			Triple newTriple1 = Triple.create(subjectVar, predicate, 
					Var.alloc("o" + level + "" + count));
			triplesStack.add(newTriple1);
			++count;
		}
	}

	/**
	 * @param currentTriple
	 * @param predicateValues
	 * @param basicPattern
	 * @return
	 */
	private Set<Node> addBoundTriplePatterns(Triple currentTriple, Var subjectVar, Set<Pair<Node, Node>> predicateValues, int level,
			BasicPattern basicPattern) {
		Set<Node> excludedProperties = new LinkedHashSet<>();
		for (Pair<Node, Node> cond : predicateValues) {
			// Check that we do it only for predicates with URI ranges
			basicPattern.add(Triple.create(subjectVar, cond.getLeft(), cond.getRight()));
			excludedProperties.add(cond.getLeft());
		}

		return excludedProperties;
	}

	/**
	 * @param currentTriple
	 * @return
	 */
	private Var getDanglingVariable(Triple currentTriple) {
		String subject = currentTriple.getSubject().toString();
		String object = currentTriple.getObject().toString();
		if (currentTriple.getSubject().isVariable() && subject.startsWith("?o"))
			if (object.startsWith("?o")) {
				int objectId = Integer.parseInt(object.substring(2, object.length() - 1));
				int subjectId = Integer.parseInt(subject.substring(2, subject.length() - 1));
				if (objectId > subjectId) {
					return Var.alloc(currentTriple.getObject());
				} else {
					return Var.alloc(currentTriple.getSubject());					
				}
			} else {
				return Var.alloc(currentTriple.getSubject());
			}
		else
			return Var.alloc(currentTriple.getObject());
	}

	/**
	 * @param triple
	 * @param uri
	 * @return
	 */
	private Triple bindTriple(Triple triple, Node uri) {
		Var danglingVar = getDanglingVariable(triple);
		
		if (danglingVar.equals(Var.alloc(triple.getSubject()))) {
			return new Triple(uri, triple.getPredicate(), 
					triple.getObject());
		} else {
			return new Triple(triple.getSubject(), triple.getPredicate(), 
					uri);
		}
	}

	/**
	 * @param basicPattern 
	 * @param currentTriple
	 * @return
	 */
	private Set<String> getBindingsSet(BasicPattern basicPattern, Triple currentTriple) {
		Set<String> result = new LinkedHashSet<>();		
		BasicPattern bp = new BasicPattern();
		bp.addAll(basicPattern);
		Var var = getDanglingVariable(currentTriple);
		bp.add(currentTriple);
		
		Op op = new OpProject(new OpBGP(bp), Arrays.asList(var));
		QueryIterator qIter = Algebra.exec(op, provenanceGraph) ;
		
		while (qIter.hasNext()) {
			Binding b = qIter.next();
			result.add(b.get(Var.alloc(var)).toString());
			//result.add(b.get(Var.alloc(currentTriple.getObject())).toString());
		}
		
		return result;
	}

	/**
	 * @param predicateObjects
	 * @param qIter
	 */
	private void intersectBindings(String binding, Var p, Var o, QueryIterator qIter, Set<Pair<Node, Node>> predicateObjects, Set<Node> predicates) {
		List<Pair<Node, Node>> retainPOSet = new ArrayList<>();
		Set<Node> retainPSet = new LinkedHashSet<>();
		while (qIter.hasNext()) {
			Binding b = qIter.nextBinding();
			Node np = b.get(p);
			Node no = b.get(o);
			// Avoid predicates leading to reflexive triples
			if (binding.equals(no.toString())) {
				continue;
			}
			
			retainPOSet.add(new MutablePair<Node, Node>(np, no));
			retainPSet.add(np);
		}
		predicateObjects.retainAll(retainPOSet);
		predicates.retainAll(retainPSet);
		
	}

	/**
	 * @param predicateObjects
	 * @param qIter
	 */
	private void addBindings(String binding, Var p, Var o, QueryIterator qIter, Set<Pair<Node, Node>> predicateObjects, Set<Node> predicates) {
		while (qIter.hasNext()) {
			Binding b = qIter.nextBinding();
			Node np = b.get(p);
			Node no = b.get(o);
			// Avoid predicates leading to reflexive things
			if (binding.equals(no.toString())) {
				continue;
			}
			predicateObjects.add(new MutablePair<Node, Node>(np, no));
			predicates.add(np);
		}
		
	}

}
