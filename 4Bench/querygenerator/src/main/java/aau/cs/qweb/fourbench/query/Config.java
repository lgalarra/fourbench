/**
 * File: Config.java
 */
package aau.cs.qweb.fourbench.query;

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.LinkedHashSet;
import java.util.List;
import java.util.Set;

import org.apache.jena.riot.Lang;
import org.apache.jena.riot.RDFLanguages;

/**
 * @author galarraga
 *
 */
public class Config {
	public static final String provenanceGraphURI = "http://fourbench.org/provenance/graph/";
	
	private static Config instance;
	
	public List<String> inputQueryFiles;
	
	public List<String> inputDataFiles;
	
	public Set<Float> coverageValues;
	
	public String storagePath;
	
	public String outputDir;
	
	public int numberOfModifiedQueries;
	
	public Lang inputLanguage;
	
	private Config() {
		inputDataFiles = new ArrayList<>();
		inputQueryFiles = new ArrayList<>();
		coverageValues = new LinkedHashSet<>();
		coverageValues.add(new Float(0.25));
		storagePath = null;
		outputDir = Paths.get("").toAbsolutePath().toString();
		numberOfModifiedQueries = 1;
		inputLanguage = RDFLanguages.NQUADS;
	}
	
	public static Config getInstance() {
		if (instance == null) {
			instance = new Config();
		}
		
		return instance;
	}

	/**
	 * @param optionValue
	 * @throws IOException 
	 * @throws FileNotFoundException 
	 */
	public void parseFromFile(String inputFileName) throws FileNotFoundException, IOException {
    	try (BufferedReader br = new BufferedReader(new FileReader(inputFileName))) {

			String fileLine;
			while ((fileLine = br.readLine()) != null) {
				if (fileLine.startsWith("query")) {
					inputQueryFiles.add(fileLine.split("=")[1].trim());
				} else if (fileLine.startsWith("data")) {
					inputDataFiles.add(fileLine.split("=")[1].trim());
				} else if (fileLine.startsWith("storagePath")) {
					storagePath = fileLine.split("=")[1].trim();
				} else if (fileLine.startsWith("outputDir")) {
					outputDir = fileLine.split("=")[1].trim();
				} else if (fileLine.startsWith("numberOfModifiedQueries")) {
					numberOfModifiedQueries = Integer.parseInt(fileLine.split(" ")[1].trim());
				} else if (fileLine.startsWith("inputLanguage")) {
					parseLanguage(fileLine.split("=")[1].trim());
				} else if (fileLine.startsWith("coverageValue")) {
					coverageValues.add(Float.parseFloat(fileLine.split("=")[1].trim()));
				}
			}
		}
	}
	
	/**
	 * @param format
	 */
	private void parseLanguage(String format) {
		switch (format) {
		case "ttl" :
			inputLanguage = RDFLanguages.TTL;
			break;
		case "n4" : case "nq" :
			inputLanguage = RDFLanguages.NQUADS;
			break;
		case "tsv" :
			inputLanguage = RDFLanguages.CSV;
			break;
		case "n3" :
			inputLanguage = RDFLanguages.NTRIPLES;
			break;
		}
		
	}

	@Override
	public String toString() {
		StringBuilder builder = new StringBuilder();
		builder.append("Config: { ");
		builder.append("Input queries: ");
		builder.append(inputQueryFiles);
		builder.append(", ");
		builder.append("Data: ");
		builder.append(inputDataFiles);
		builder.append(", ");
		builder.append("Storage Path: ");
		builder.append(storagePath);
		builder.append(", ");
		builder.append("Output directory: ");
		builder.append(outputDir);
		builder.append(", ");
		builder.append("Number of provenance queries: " );
		builder.append(numberOfModifiedQueries);
		builder.append(", ");
		builder.append("Coverage values: ");
		builder.append(coverageValues);
		builder.append("}");
	
		return builder.toString();
	}

}
