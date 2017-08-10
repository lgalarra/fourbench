/**
 * File: ProvenancePathsGenerator.java
 * 
 */
package aau.cs.qweb.fourbench.query;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collection;
import java.util.Collections;
import java.util.HashMap;
import java.util.Iterator;
import java.util.LinkedHashMap;
import java.util.LinkedHashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;

import org.apache.jena.graph.Node;
import org.apache.jena.graph.Node_Variable;
import org.apache.jena.graph.Triple;
import org.apache.jena.query.Dataset;
import org.apache.jena.sparql.algebra.Algebra;
import org.apache.jena.sparql.algebra.op.OpBGP;
import org.apache.jena.sparql.core.Var;
import org.apache.jena.sparql.engine.QueryIterator;
import org.apache.jena.sparql.engine.binding.Binding;
import org.apache.jena.sparql.engine.binding.BindingFactory;
import org.apache.jena.sparql.engine.binding.BindingMap;
import org.mapdb.BTreeMap;

/**
 * @author galarraga
 *
 */
public class ProvenancePathsGenerator {
	Dataset dataset;
	BTreeMap<String, String> idx;
	
	public static final String UnionGraph = "urn:x-arq:UnionGraph";
	
	public ProvenancePathsGenerator(Dataset dataset, BTreeMap<String, String> idx) {
		this.dataset = dataset;
		this.idx = idx;
	}
	
	/**
	 * @param bgps
	 * @param dataset
	 * @return
	 */
	public List<Collection<List<String>>> generate(List<Set<Triple>> bgps) {
		List<Collection<List<String>>> result = new ArrayList<>();
		
		for (Set<Triple> tripleList : bgps) {
			List<Binding> computedBindings = new ArrayList<>();
			Map<String, List<String>> provenancePaths = new LinkedHashMap<>();
			boolean firstTriplePattern = true;
			for (Triple t : tripleList) {
				computedBindings = updatePaths(t, firstTriplePattern, computedBindings, provenancePaths);
				// If there are no results, quit
				if (computedBindings.isEmpty()) {
					break;
				}
				firstTriplePattern = false;
			}
			result.add(provenancePaths.values());
		}
		
		return result;
	}
	
	/**
	 * 
	 */
	private List<Binding> updatePaths(Triple triple, boolean isFirstTriplePattern, 
			List<Binding> computedBindings, 
			Map<String, List<String>> provenancePaths) {
		if (computedBindings.isEmpty() && isFirstTriplePattern) {
			getInitialBindings(triple, computedBindings, provenancePaths);
			return computedBindings;
		} else {
			// Build an index on the common variables to run a hashjoin
			if (computedBindings.isEmpty()) {
				return Collections.emptyList();
			}
			
			Binding binding = computedBindings.get(0);
			Set<Var> commonVariables = getCommonVariables(binding, triple);
			Set<Var> allVariables = getVariables(binding);
			// This guarantees that tmpIdx and provenancePaths have the same keys
			Map<String, List<Binding>> tmpIdx = buildIndexes(commonVariables, allVariables, computedBindings, provenancePaths);
			return getJoinedBindings(triple, commonVariables, tmpIdx, provenancePaths);
		}
	}

	/**
	 * @param binding
	 * @return
	 */
	private Set<Var> getVariables(Binding binding) {
		Set<Var> result = new LinkedHashSet<>();
		Iterator<Var> vit = binding.vars();
		while (vit.hasNext()) {
			result.add(vit.next());
		}
		
		return result;
	}

	/**
	 * @param triple
	 * @param computedBindings
	 * @param tmpIdx
	 * @param provenancePaths
	 */
	private List<Binding> getJoinedBindings(Triple triple, Set<Var> commonVariables, Map<String, List<Binding>> tmpIdx, Map<String, List<String>> provenancePaths) {
		List<Binding> result = new ArrayList<>();
		OpBGP singletonBGP = new OpBGP();
		singletonBGP.getPattern().add(triple);
		QueryIterator qIter = Algebra.execRef(singletonBGP, 
				dataset.getNamedModel(UnionGraph));
		Set<Var> resultVars = getJoinResultVariables(commonVariables, triple);
		Map<String, List<String>> newProvenancePaths = new LinkedHashMap<>();
		for ( ; qIter.hasNext() ; )
        {
            Binding b = qIter.nextBinding() ;
            String key = getIndexKey(commonVariables, b);
            List<Binding> values = tmpIdx.get(key);
            List<String> provenancePath = provenancePaths.get(key);
            
            if (values != null) {
            	for (Binding value : values) {
            		// Create the joined binding
            		Binding join = join(b, value, resultVars); 
            		result.add(join);
            		String provenanceId = binding2ProvenanceId(b, triple);
            		List<String> extendedPath = new ArrayList<>(provenancePath);
            		extendedPath.add(provenanceId);
            		newProvenancePaths.put(getIndexKey(resultVars, join), extendedPath);
            	}
            }
        }
		
		provenancePaths.clear();
		provenancePaths.putAll(newProvenancePaths);
        
		return result;
		
	}
	
	/**
	 * @param commonVariables
	 * @param triple
	 * @return
	 */
	private Set<Var> getJoinResultVariables(Set<Var> commonVariables, Triple triple) {
		Set<Var> result = new LinkedHashSet<>(commonVariables);
		
		Node subject = triple.getSubject();
		if (subject.isVariable()) {
			result.add(Var.alloc(subject));
		}
		
		Node predicate = triple.getPredicate();
		if (predicate.isVariable()) {
			result.add(Var.alloc(predicate));
		}
		
		Node object = triple.getObject();
		if (predicate.isVariable()) {
			result.add(Var.alloc(object));
		}
		
		return result;
	}

	/**
	 * @param b
	 * @param value
	 * @param resultVars
	 */
	private Binding join(Binding b, Binding value, Set<Var> resultVars) {
		BindingMap result = BindingFactory.create();
		for (Var v : resultVars) {
			Node n = b.get(v);
			if (n == null) {
				n = value.get(v);
			}
			result.add(v, n);
		}
		
		return result;
	}

	private String getIndexKey(Collection<Var> vars, Binding b) {
		StringBuilder builder = new StringBuilder();
		List<String> parts = new ArrayList<>();
		for (Var v : vars) {
			parts.add(v + "=" + b.get(v));
		}
		Collections.sort(parts);
		for (String part : parts) {
			builder.append(part);
		}
		return builder.toString();
	}
	

	/**
	 * @param commonVariables
	 * @param computedBindings
	 * @return
	 */
	private Map<String, List<Binding>> buildIndexes(Set<Var> commonVariables, 
			Set<Var> allVariables,
			List<Binding> computedBindings, Map<String, List<String>> provenancePaths) {
		Map<String, List<Binding>> result = new HashMap<>();
		boolean project = commonVariables.size() < allVariables.size();
		
		for (Binding b : computedBindings) {
			String idxBindingsKey = getIndexKey(commonVariables, b);
			
			if (!result.containsKey(idxBindingsKey)) {
				result.put(idxBindingsKey, new ArrayList<>());
			}
			
			result.get(idxBindingsKey).add(b);
			
			// Adjust the provenance path index to consider only the common variables
			if (project) {
				String idxProvenanceKey = getIndexKey(allVariables, b);
				List<String> provenanceIds = provenancePaths.get(idxProvenanceKey);
				provenancePaths.remove(idxProvenanceKey);
				provenancePaths.put(idxBindingsKey, provenanceIds);
			}
	
		}
		
		return result;
	}

	/**
	 * @param binding
	 * @param triple
	 * @return
	 */
	private Set<Var> getCommonVariables(Binding binding, Triple triple) {
		Set<Var> result = new LinkedHashSet<>();
		Iterator<Var> it = binding.vars();
		while (it.hasNext()) {
			Node subject = triple.getSubject();
			Var variable = it.next();
			if (subject instanceof Node_Variable && 
					variable.toString().equals(subject.toString())) {
				result.add(variable);
			}
		}
		return result;
	}

	/**
	 * @param triple
	 * @param computedBindings
	 */
	private void getInitialBindings(Triple triple, List<Binding> computedBindings, Map<String, List<String>> provenancePaths) {
		OpBGP singletonBGP = new OpBGP();
		singletonBGP.getPattern().add(triple);
		QueryIterator qIter = Algebra.execRef(singletonBGP, 
				dataset.getNamedModel(UnionGraph));
		List<Var> vars = new ArrayList<>();
		if (triple.getSubject().isVariable()) {
			vars.add(Var.alloc(triple.getSubject()));
		}
		
		if (triple.getPredicate().isVariable()) {
			vars.add(Var.alloc(triple.getPredicate()));
		}
		
		if (triple.getObject().isVariable()) {
			vars.add(Var.alloc(triple.getObject()));
		}
		
		
		for ( ; qIter.hasNext() ; )
        {
            Binding b = qIter.nextBinding() ;	
            computedBindings.add(b); 
            provenancePaths.put(getIndexKey(vars, b), 
            		Arrays.asList(binding2ProvenanceId(b, triple))
            		);
        }
        qIter.close() ;		
	}

	/**
	 * @param b
	 * @param triple
	 * @return
	 */
	private String binding2ProvenanceId(Binding b, Triple triple) {
		Node subject, predicate, object;
		if (triple.getSubject() instanceof Node_Variable) {
			subject = b.get(Var.alloc(triple.getSubject()));
		} else {
			subject = triple.getSubject();
		}
		
		if (triple.getPredicate() instanceof Node_Variable) {
			predicate = b.get(Var.alloc(triple.getPredicate()));
		} else {
			predicate = triple.getPredicate();
		}
		
		if (triple.getObject() instanceof Node_Variable) {
			object = b.get(Var.alloc(triple.getObject()));
		} else {
			object = triple.getObject();
		}
		
		Triple boundTriple = new Triple(subject, predicate, object);
		
		return idx.get(boundTriple.toString());
	}

}
