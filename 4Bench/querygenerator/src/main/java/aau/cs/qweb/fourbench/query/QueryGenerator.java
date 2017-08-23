
package aau.cs.qweb.fourbench.query;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.Collections;
import java.util.LinkedHashMap;
import java.util.LinkedHashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;

import org.apache.commons.cli.CommandLine;
import org.apache.commons.cli.CommandLineParser;
import org.apache.commons.cli.DefaultParser;
import org.apache.commons.cli.HelpFormatter;
import org.apache.commons.cli.Options;
import org.apache.commons.cli.ParseException;
import org.apache.commons.io.FileUtils;
import org.apache.jena.graph.Triple;
import org.apache.jena.query.Dataset;
import org.apache.jena.query.Query;
import org.apache.jena.query.QueryFactory;
import org.apache.jena.query.Syntax;
import org.apache.jena.riot.RDFDataMgr;
import org.apache.jena.riot.system.StreamRDFBase;
import org.apache.jena.sparql.algebra.Algebra;
import org.apache.jena.sparql.algebra.Op;
import org.apache.jena.sparql.algebra.op.OpBGP;
import org.apache.jena.sparql.algebra.op.OpUnion;
import org.apache.jena.sparql.core.Quad;
import org.apache.jena.tdb.TDBFactory;
import org.mapdb.BTreeMap;
import org.mapdb.DB;
import org.mapdb.DBMaker;
import org.mapdb.Serializer;

import aau.cs.qweb.fourbench.utils.StringUtils;


public class QueryGenerator extends StreamRDFBase {
	Dataset dataset;
	Config config;
	DB triples2ProvenanceDB;
	BTreeMap<String, String> triples2ProvenanceIdx;
	Map<String, String> buffer;
	
	private static final int bufferSize = 10000;
	
	/**
	 * @param model
	 * @param config
	 */
	public QueryGenerator(Dataset model, Config config) {
		this.dataset = model;
		this.config = config;
		String dbFileName = StringUtils.getPath(this.config.storagePath, "index.mdb");
		File dbFile = new File(dbFileName);
		if (dbFile.exists()) {
			if (!dbFile.delete()) {
				System.err.println("File " + dbFileName + " already exists and could not be deleted.");
			}
		}
		
		triples2ProvenanceDB = DBMaker.fileDB(StringUtils.getPath(
				this.config.storagePath, "index.mdb")).make();
		triples2ProvenanceIdx = triples2ProvenanceDB.treeMap("map")
		        .keySerializer(Serializer.STRING)
		        .counterEnable()
		        .valueSerializer(Serializer.STRING)
		        .createOrOpen();
		buffer = new LinkedHashMap<>();
	}

	private static void printHelp(ParseException exp, Options options) {
		String header = "";
		HelpFormatter formatter = new HelpFormatter();
		if (exp != null) {
			header = "Unexpected exception:" + exp.getMessage();
		}
		
		formatter.printHelp("QueryGenerator", header, options, null, true);
	}
	
	/**
	 * 
	 * @param conf
	 * @return
	 */
	public static QueryGenerator getQueryGenerator(Config conf) {
		String location = conf.outputDir + 
        		(conf.outputDir.charAt(conf.outputDir.length() - 1) == '/' ? "" : "/") 
        		+ "dataset";
		try {
            File f = new File(location);
            if (f.exists()) {
	            if (f.isDirectory()) {
	            	FileUtils.cleanDirectory(f); //clean out directory (this is optional -- but good know)
	            }
	            FileUtils.forceDelete(f); //delete directory
	            FileUtils.forceMkdir(f); //create directory
            }
        } catch (IOException e) {
            e.printStackTrace();
            return null;
        } 
		
		Dataset rdfModel = TDBFactory.createDataset(location);
		QueryGenerator qGenerator = new QueryGenerator(rdfModel, conf);
		
		for (String iFile : conf.inputDataFiles) {
			System.out.println("Reading file " + iFile);
			RDFDataMgr.parse(qGenerator, iFile, conf.inputLanguage);
		}
		
		qGenerator.flush();
		System.out.println("Index triple->provenance-id contains " + qGenerator.getIndexSize() + " triples");
		
		return qGenerator;
	}
	
	/**
	 * @return
	 */
	private long getIndexSize() {
		// TODO Auto-generated method stub
		return triples2ProvenanceIdx.sizeLong();
	}

	/**
	 * 
	 */
	private void flush() {
		System.out.println("Flushing the triples-to-provenance-ids buffer");
		triples2ProvenanceIdx.putAll(buffer);
		buffer.clear();
	}

	@Override
    public void quad(Quad quad) {
		dataset.asDatasetGraph().add(quad);
		
		String graph = quad.getGraph().toString();
		// Do not index the provenance graph
		if (graph.equals(Config.provenanceGraphURI)) {
			return;
		}
		
		buffer.put(quad.asTriple().toString(), graph);
		
		if (buffer.size() >= bufferSize) {
			flush();
		}
    }

	
	/**
	 * 
	 * @param args
	 */
	public static void main(String[] args) {
		CommandLineParser parser = new DefaultParser();
		Config config = Config.getInstance();
		
		
		// create the Options
		Options options = new Options();
		options.addOption("h", "help", false, "Display this message." );
		options.addOption("c", "config", true, "Path to config file");
		try {
		    CommandLine line = parser.parse( options, args );
				    
		    if (line.hasOption( "help" )) {
		    	printHelp(null,options);
		    	System.exit(0);
			} 

			
		    if (line.hasOption("config")) {
				try {
					config.parseFromFile(line.getOptionValue("config"));
				} catch (IOException e) {
					e.printStackTrace();
				}
				
		    }
		}
		catch( ParseException exp ) {
			printHelp(exp, options);
		} 
		
		QueryGenerator qGenerator = getQueryGenerator(config);
		for (String iQueryFile : config.inputQueryFiles) {
			String iQuery;
			try {
				iQuery = readFileContents(iQueryFile);
				System.out.println("Processing query");
				System.out.println(iQuery);
				String oQuery = qGenerator.extendQuery(iQuery);
				writeExtendedQuery(oQuery, getExtendedQueryOutputPath(iQueryFile, config));
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
		
	}

	/**
	 * @param oQuery
	 * @param extendedQueryOutputPath
	 * @throws IOException 
	 */
	private static void writeExtendedQuery(String oQuery, String outputFile) throws IOException {
		BufferedWriter bw = new BufferedWriter(new FileWriter(outputFile));
		bw.write(oQuery);
		bw.close();
	}
	

	/**
	 * @param iQuery
	 * @return
	 */
	private String extendQuery(String iQuery) {
		// TODO: 
		// 1. Convert the query into an algebra expression
		Query query = QueryFactory.create(iQuery, Syntax.syntaxSPARQL);
		Op op = Algebra.compile(query);
		List<Set<Triple>> bgps = ImprovedOpWalker.walk(op);
		System.out.println("Algebra representation");
		System.out.print(op);
		ProvenancePathsGenerator pathsGenerator = 
				new ProvenancePathsGenerator(dataset, triples2ProvenanceIdx);
		// Get the provenance identifier paths
		List<List<List<String>>> provenancePaths = pathsGenerator.generate(bgps);
		for (Float coverageValue : config.coverageValues) {
			List<List<List<String>>> provenanceIds = filterPaths(coverageValue.floatValue(), provenancePaths);
			Op provenanceQuery = computeProvenanceQuery(provenanceIds);
			System.out.println("coverage = " + coverageValue + "  " + provenanceQuery);
			
		}
		
		System.out.println(provenancePaths);
		// 
		return iQuery;
	}

	/**
	 * @param provenanceIds
	 * @return
	 */
	private Op computeProvenanceQuery(List<List<List<String>>> provenanceIds) {
		ProvenanceQueryGenerator generator = new ProvenanceQueryGenerator(dataset);
		List<OpBGP> resultBGPs = new ArrayList<>();
		for (List<List<String>> cluster : provenanceIds) {
			Set<String> flattenedSet = flatten(cluster);
			OpBGP bgp = generator.computeBGP(flattenedSet);
			System.out.println("Computed bgp " + bgp);
			if (!bgp.getPattern().isEmpty())
				resultBGPs.add(bgp);
		}
		
		return unionBGPs(resultBGPs);
	}

	/**
	 * @param resultBGPs
	 * @return
	 */
	private Op unionBGPs(List<OpBGP> resultBGPs) {
		if (resultBGPs.size() == 1) {
			return resultBGPs.get(0);
		} else if (resultBGPs.size() >= 2) {
			Op lastUnion = new OpUnion(resultBGPs.get(0), resultBGPs.get(1));
			for (int i = 2; i < resultBGPs.size(); ++i) {
				Op currentUnion = new OpUnion(lastUnion, resultBGPs.get(i));
				lastUnion = currentUnion;
			}
			return lastUnion;
		}
		
		return null;
		
	}

	/**
	 * @param cluster
	 * @return
	 */
	private Set<String> flatten(List<List<String>> cluster) {
		Set<String> result = new LinkedHashSet<>();
		
		for (List<String> path : cluster) {
			result.addAll(path);
		}
		
		return result;
	}

	/**
	 * @param floatValue
	 * @return
	 */
	private List<List<List<String>>> filterPaths(float ratio, List<List<List<String>>> provenancePaths) {
		List<List<List<String>>> subclusters = new ArrayList<>(); 
		for (List<List<String>> cluster : provenancePaths) {
			Collections.shuffle(cluster);
			int size = (int) Math.ceil((double)cluster.size() * ratio);
			List<List<String>> subcluster = new ArrayList<>();
			for (int i = 0; i < size; ++i) {
				subcluster.add(cluster.get(i));
			}
			subclusters.add(subcluster);
			
		}
		
		return subclusters;
	}

	/**
	 * @param iQueryFile
	 * @param config
	 * @return
	 */
	private static String getExtendedQueryOutputPath(String iQueryFile, Config config) {
		File file = new File(iQueryFile);
		return config.outputDir + 
				(config.outputDir.charAt(config.outputDir.length() - 1) == '/' ? 
						file.getName() : "/" + file.getName())
				+ "extended";
	}

	/**
	 * @param iQueryFile
	 * @return
	 * @throws IOException 
	 */
	private static String readFileContents(String iQueryFile) throws IOException {
		return new String(Files.readAllBytes(Paths.get(iQueryFile)), StandardCharsets.UTF_8);
	}

}
