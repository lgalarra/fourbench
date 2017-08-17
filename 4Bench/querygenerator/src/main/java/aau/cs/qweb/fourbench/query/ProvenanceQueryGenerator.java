/**
 * File: ProvenanceQueryGenerator.java
 * 
 */
package aau.cs.qweb.fourbench.query;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.LinkedHashSet;
import java.util.LinkedList;
import java.util.List;
import java.util.Queue;
import java.util.Set;

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

	public static final String provenanceGraphURI = "http://fourbench.org/provenance/graph/";
	
	Dataset dataset;
	
	Model provenanceGraph;
	
	/**
	 * @param dataset
	 */
	public ProvenanceQueryGenerator(Dataset dataset) {
		this.dataset = dataset;
		this.provenanceGraph = dataset.getNamedModel(provenanceGraphURI);
	}

	/**
	 * @param flattenedSet
	 * @return
	 */
	public OpBGP computeBGP(Set<String> flattenedSet) {	
		BasicPattern basicPattern = new BasicPattern();
		int level = 0;
		Queue<Triple> triplesQueue = new LinkedList<>();
		Triple currentTriple = null;
		
		while (true) {
			boolean firstIteration = (currentTriple == null);
			Set<String> currentSet = null;
			Var var1 = Var.alloc("p");
			Var var2 = Var.alloc("o");
			
			if (firstIteration) {
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
				if (firstIteration) {
					boundPattern = new Triple(NodeFactory.createURI(binding), var1, var2);
				} else {
					boundPattern = bindTriple(currentTriple, NodeFactory.createURI(binding));
					Triple nextPattern = new Triple(NodeFactory.createURI(binding), var1, var2);
					bp.add(nextPattern);
				}
				
				bp.addAll(basicPattern);
				bp.add(boundPattern);
				System.out.println("Trying " + bp);
				Op op = new OpProject(new OpBGP(bp), Arrays.asList(var1, var2));
				QueryIterator qIter = Algebra.exec(op, provenanceGraph) ;
				if (firstSetOfBindings) {
					firstSetOfBindings = false;
					addBindings(var1, var2, qIter, predicateValues, predicates);
				} else {
					intersectBindings(var1, var2, qIter, predicateValues, predicates);
				}
				qIter.close();
				
				// No reason to continue
				if (predicates.isEmpty() && predicateValues.isEmpty()) {
					break;
				}
			}
			
			// If we cannot add specialized versions of this triple 
			// pattern add it and clear the queue
			if (currentTriple != null &&
					predicates.isEmpty() && predicateValues.isEmpty()) {
				basicPattern.add(currentTriple);
				triplesQueue.clear();
			}
			
			// Otherwise go for the specialized versions
			Var subjectVar = Var.alloc("i" + level);
			Set<Node> excludedProperties = new LinkedHashSet<>();
			for (Pair<Node, Node> cond : predicateValues) {
				// Check that we do it only for predicates with URI ranges
				basicPattern.add(Triple.create(subjectVar, cond.getLeft(), cond.getRight()));
				excludedProperties.add(cond.getLeft());
			}
			
			
			// And try to specialize the remaining properties
			int count = 0;			
			predicates.removeAll(excludedProperties);
			for (Node predicate : predicates) {
				Triple newTriple1 = Triple.create(subjectVar, predicate, 
						Var.alloc("o" + level + "" + count));
				triplesQueue.add(newTriple1);
				++count;
				Triple newTriple2 = Triple.create(Var.alloc("o" + level + "" + count), 
						predicate, subjectVar);
				triplesQueue.add(newTriple2);				
			}
			
			if (triplesQueue.isEmpty())
				break;
			
			currentTriple = triplesQueue.poll();
			++level;
		}
		
		return new OpBGP(basicPattern);
	}

	/**
	 * @param currentTriple
	 * @param createURI
	 * @return
	 */
	private Triple bindTriple(Triple currentTriple, Node createURI) {
		if (currentTriple.getSubject().toString().startsWith("?o")) {
			return new Triple(createURI, currentTriple.getPredicate(), 
					currentTriple.getObject());
		} else {
			return new Triple(currentTriple.getSubject(), currentTriple.getPredicate(), 
					createURI);
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
		Var var = null;
		
		if (currentTriple.getSubject().toString().startsWith("?o"))
			var = Var.alloc(currentTriple.getSubject());
		else
			var = Var.alloc(currentTriple.getObject());
		
		bp.add(currentTriple);
		
		Op op = new OpProject(new OpBGP(bp), Arrays.asList(var));
		QueryIterator qIter = Algebra.exec(op, provenanceGraph) ;
		
		while (qIter.hasNext()) {
			Binding b = qIter.next();
			result.add(b.get(Var.alloc(currentTriple.getObject())).toString());
		}
		
		return result;
	}

	/**
	 * @param predicateObjects
	 * @param qIter
	 */
	private void intersectBindings(Var p, Var o, QueryIterator qIter, Set<Pair<Node, Node>> predicateObjects, Set<Node> predicates) {
		List<Pair<Node, Node>> retainPOSet = new ArrayList<>();
		Set<Node> retainPSet = new LinkedHashSet<>();
		while (qIter.hasNext()) {
			Binding b = qIter.nextBinding();
			Node np = b.get(p);
			Node no = b.get(o);
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
	private void addBindings(Var p, Var o, QueryIterator qIter, Set<Pair<Node, Node>> predicateObjects, Set<Node> predicates) {
		while (qIter.hasNext()) {
			Binding b = qIter.nextBinding();
			Node np = b.get(p);
			Node no = b.get(o);
			predicateObjects.add(new MutablePair<Node, Node>(np, no));
			predicates.add(np);
		}
		
	}

}
