/**
 * File: ProvenanceQueryGenerator.java
 * 
 */
package aau.cs.qweb.fourbench.query;

import java.util.ArrayList;
import java.util.LinkedHashSet;
import java.util.List;
import java.util.Set;

import org.apache.commons.lang3.tuple.MutablePair;
import org.apache.commons.lang3.tuple.Pair;
import org.apache.jena.graph.Node;
import org.apache.jena.graph.NodeFactory;
import org.apache.jena.graph.Node_URI;
import org.apache.jena.graph.Node_Variable;
import org.apache.jena.graph.Triple;
import org.apache.jena.query.Dataset;
import org.apache.jena.rdf.model.Model;
import org.apache.jena.sparql.algebra.Algebra;
import org.apache.jena.sparql.algebra.Op;
import org.apache.jena.sparql.algebra.op.OpBGP;
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
	
	/**
	 * @param dataset
	 */
	public ProvenanceQueryGenerator(Dataset dataset) {
		this.dataset = dataset;
	}

	/**
	 * @param flattenedSet
	 * @return
	 */
	public OpBGP computeBGP(Set<String> flattenedSet) {	
		Var varP = Var.alloc("p");
		Var varO = Var.alloc("o");
		Model provenanceGraph = dataset.getNamedModel(provenanceGraphURI);
		Set<Pair<Node, Node>> predicateObjects = new LinkedHashSet<>();
		Set<Node> predicates = new LinkedHashSet<>();
		boolean firstTime = true;
		
		for (String provId : flattenedSet) {
			Triple selectivePattern = new Triple(NodeFactory.createURI(provId), varP, varO);
			BasicPattern bp = new BasicPattern();
			Op op = new OpBGP(bp);
			bp.add(selectivePattern);
			QueryIterator qIter = Algebra.exec(op, provenanceGraph) ;
			if (firstTime) {
				firstTime = false;
				addBindings(varP, varO, qIter, predicateObjects, predicates);
				qIter.close();
			} else {
				retainBindings(varP, varO, qIter, predicateObjects, predicates);
			}
		}
		
		return null;
	}

	/**
	 * @param predicateObjects
	 * @param qIter
	 */
	private void retainBindings(Var p, Var o, QueryIterator qIter, Set<Pair<Node, Node>> predicateObjects, Set<Node> predicates) {
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
