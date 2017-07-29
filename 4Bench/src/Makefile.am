bin_PROGRAMS=4Bench
4Bench_SOURCES=4Bench.cpp utils/string.cpp conf/Conf.cpp  utils/integer.cpp utils/float.cpp \
 parsing/FileParser.cpp parsing/FileParserFactory.cpp parsing/Triple.cpp parsing/TSVFileParser.cpp parsing/ParsingStats.cpp \
 provenance/ProvenanceGraph.cpp provenance/ProvenanceGraphFactory.cpp provenance/ProvenanceSubgraph.cpp provenance/ProvenanceGraphPopulator.cpp \
 provenance/ProvenanceAssignment.cpp  provenance/ProvenanceAssignmentFactory.cpp provenance/UniformProvenanceAssignment.cpp \
 provenance/IRIBuilder.cpp provenance/ProvenanceObject.cpp provenance/Activity.cpp provenance/Entity.cpp provenance/Agent.cpp \
 provenance/PROVO.cpp provenance/RDF.cpp provenance/RDFS.cpp provenance/Vocabulary.cpp provenance/FOAF.cpp provenance/XSD.cpp \
 output/ProvenanceDump.cpp output/ProvenanceDumpFactory.cpp output/QuadTSVProvenanceDump.cpp \
 datatypes/DataType.cpp datatypes/DataValue.cpp datatypes/DataValueBuilder.cpp
4Bench_CPPFLAGS=-I$(top_srcdir)/include
AM_CPPFLAGS=$(BOOST_CPPFLAGS)
AM_LDFLAGS=$(BOOST_LDFLAGS)
4Bench_LDADD=$(BOOST_PROGRAM_OPTIONS_LIB)