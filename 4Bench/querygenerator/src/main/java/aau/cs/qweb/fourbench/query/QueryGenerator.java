
package aau.cs.qweb.fourbench.query;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.List;

import org.apache.commons.cli.CommandLine;
import org.apache.commons.cli.CommandLineParser;
import org.apache.commons.cli.DefaultParser;
import org.apache.commons.cli.HelpFormatter;
import org.apache.commons.cli.Options;
import org.apache.commons.cli.ParseException;
import org.apache.commons.io.FileUtils;
import org.apache.jena.graph.Triple;
import org.apache.jena.query.Dataset;
import org.apache.jena.query.DatasetFactory;
import org.apache.jena.query.Query;
import org.apache.jena.query.QueryFactory;
import org.apache.jena.query.Syntax;
import org.apache.jena.riot.RDFDataMgr;
import org.apache.jena.sparql.algebra.Algebra;
import org.apache.jena.sparql.algebra.Op;
import org.apache.jena.tdb.TDBFactory;
import org.apache.jena.tdb.sys.TDBMaker;


public class QueryGenerator {
	Dataset dataset;
	Config config;
	
	/**
	 * @param model
	 * @param config
	 */
	public QueryGenerator(Dataset model, Config config) {
		this.dataset = model;
		this.config = config;
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
	public static Dataset loadDataset(Config conf) {
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
		
		for (String iFile : conf.inputDataFiles) {
			System.out.println("Reading file " + iFile);
			RDFDataMgr.read(rdfModel, iFile, conf.inputLanguage);
		}
		
		return rdfModel;
	}

	
	/**
	 * 
	 * @param args
	 */
	public static void main(String[] args) {
		CommandLineParser parser = new DefaultParser();
		Config config = Config.getInstance();
		QueryGenerator qGenerator = null;
		
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
		
		Dataset model = loadDataset(config);
		qGenerator = new QueryGenerator(model, config);
		for (String iQueryFile : config.inputQueryFiles) {
			String iQuery;
			try {
				iQuery = readFileContents(iQueryFile);
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
		List<List<Triple>> bgps = ImprovedOpWalker.walk(op);
		System.out.print(op);
		System.out.println(bgps);
		// Get the provenance identifier paths
		List<List<String>> provenancePaths = ProvenancePathsGenerator.generate(bgps, dataset);
	
		return iQuery;
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
