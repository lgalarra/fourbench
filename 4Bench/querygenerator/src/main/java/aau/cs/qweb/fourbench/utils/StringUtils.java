/**
 * File: StringUtils.java
 * 
 */
package aau.cs.qweb.fourbench.utils;

/**
 * @author galarraga
 *
 */
public class StringUtils {
	
	public static String getPath(String prefix, String name) {
		char lastChar = prefix.charAt(prefix.length() - 1);
		return prefix + (lastChar == '/' ? "" : "/") + name;
	}

}
