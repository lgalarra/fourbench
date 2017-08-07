
package aau.cs.qweb.fourbench.query;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.List;

import org.apache.commons.cli.CommandLine;
import org.apache.commons.cli.CommandLineParser;
import org.apache.commons.cli.DefaultParser;
import org.apache.commons.cli.HelpFormatter;
import org.apache.commons.cli.Options;
import org.apache.commons.cli.ParseException;
import org.apache.jena.graph.Triple;
import org.apache.jena.query.Dataset;
import org.apache.jena.query.DatasetFactory;
import org.apache.jena.query.Query;
import org.apache.jena.query.QueryFactory;
import org.apache.jena.query.Syntax;
import org.apache.jena.rdf.model.Model;
import org.apache.jena.rdf.model.ModelFactory;
import org.apache.jena.riot.RDFDataMgr;
import org.apache.jena.sparql.algebra.Algebra;
import org.apache.jena.sparql.algebra.Op;
import org.apache.jena.sparql.algebra.OpVisitor;
import org.apache.jena.sparql.algebra.OpWalker;
import org.apache.jena.sparql.algebra.op.*;


public class QueryGenerator {
	Dataset model;
	Config config;
	
	/**
	 * @param model
	 * @param config
	 */
	public QueryGenerator(Dataset model, Config config) {
		this.model = model;
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
	public static Dataset loadModel(Config conf) {
		Dataset rdfModel = DatasetFactory.create();
		
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
		
		Dataset model = loadModel(config);
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
	
	class TripleVisitor implements OpVisitor {
		private List<Triple> triplePatterns = new ArrayList<>();
		
		@Override
		public void visit(OpTopN opTop) {}			
		@Override
		public void visit(OpGroup opGroup) {}
		@Override
		public void visit(OpSlice opSlice) {}
		@Override
		public void visit(OpDistinct opDistinct) {}
		@Override
		public void visit(OpReduced opReduced) {}
		@Override
		public void visit(OpProject opProject) {}
		@Override
		public void visit(OpOrder opOrder) {}			
		@Override
		public void visit(OpList opList) {}			
		@Override
		public void visit(OpDisjunction opDisjunction) {}			
		@Override
		public void visit(OpSequence opSequence) {}			
		@Override
		public void visit(OpConditional opCondition) {}
		@Override
		public void visit(OpMinus opMinus) {}			
		@Override
		public void visit(OpDiff opDiff) {}			
		@Override
		public void visit(OpUnion opUnion) {}
		@Override
		public void visit(OpLeftJoin opLeftJoin) {}
		@Override
		public void visit(OpJoin opJoin) {
		}		
		@Override
		public void visit(OpExtend opExtend) {}
		@Override
		public void visit(OpAssign opAssign) {}		
		@Override
		public void visit(OpLabel opLabel) {}
		@Override
		public void visit(OpDatasetNames dsNames) {}		
		@Override
		public void visit(OpService opService) {}
		@Override
		public void visit(OpGraph opGraph) {}
		@Override
		public void visit(OpFilter opFilter) {}		
		@Override
		public void visit(OpPropFunc opPropFunc) {}		
		@Override
		public void visit(OpProcedure opProc) {}		
		@Override
		public void visit(OpNull opNull) {}
		@Override
		public void visit(OpTable opTable) {}		
		@Override
		public void visit(OpPath opPath) {}		
		@Override
		public void visit(OpQuad opQuad) {}
		@Override
		public void visit(OpTriple opTriple) {}
		@Override
		public void visit(OpQuadBlock quadBlock) {}
		@Override 
		public void visit(OpQuadPattern quadPattern) {}
		@Override
		public void visit(OpBGP opBGP) {
			System.out.println(opBGP.getName());
			final List<Triple> triples = opBGP.getPattern().getList();
	        for (final Triple triple : triples) {
	        	triplePatterns.add(triple);
//	            System.out.println("Triple: " + triple.toString());
	        }
			
		}
		
		public List<Triple> getTriplePatterns() {
			return triplePatterns;
		}
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
		TripleVisitor visitor = new TripleVisitor();
		OpWalker.walk(op, visitor);
		List<Triple> triplePatterns = visitor.getTriplePatterns();
		System.out.print(op);
		// 2. For each triple pattern get the set of provenance identifiers
	
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
