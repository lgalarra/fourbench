/**
 * File: IdentityHashSet.java
 * 
 */
package aau.cs.qweb.fourbench.utils;

import java.util.AbstractSet;
import java.util.IdentityHashMap;
import java.util.Iterator;

/**
 * @author galarraga
 * @param <E>
 *
 */
public class IdentityHashSet<E> extends AbstractSet<E> {

	private IdentityHashMap<E, E> backMap;
	
	public IdentityHashSet() {
		backMap = new IdentityHashMap<>();
	}
	
	/* (non-Javadoc)
	 * @see java.util.AbstractCollection#iterator()
	 */
	@Override
	public Iterator<E> iterator() {
		return backMap.keySet().iterator();
	}

	/* (non-Javadoc)
	 * @see java.util.AbstractCollection#size()
	 */
	@Override
	public int size() {
		return backMap.size();
	}
	

	@Override
	public boolean add(E e) {
		return backMap.put(e, e) == null;
	}
	
	@Override
	public boolean contains(Object e) {
		return backMap.containsKey(e);
	}
	
	@Override
	public boolean remove(Object e) {
		return backMap.remove(e) != null;
	}

}
