bin_PROGRAMS = TSVFileParserTest ProvenanceGraphTest
TSVFileParserTest_SOURCES = parsing/TSVFileParserTest.cpp ../src/conf/Conf.cpp ../src/utils/string.cpp ../src/parsing/FileParser.cpp ../src/parsing/FileParserFactory.cpp ../src/parsing/Triple.cpp ../src/parsing/TSVFileParser.cpp ../src/parsing/ParsingStats.cpp
TSVFileParserTest_CPPFLAGS = -I$(top_srcdir)/include
TSVFileParserTest_LDADD = $(BOOST_PROGRAM_OPTIONS_LIB)

ProvenanceGraphTest_CPPFLAGS = $(TSVFileParserTest_CPPFLAGS)
ProvenanceGraphTest_LDADD = $(TSVFileParserTest_LDADD)
ProvenanceGraphTest_SOURCES = provenance/ProvenanceGraphTest.cpp ../src/conf/Conf.cpp ../src/utils/string.cpp ../src/utils/integer.cpp ../src/utils/float.cpp ../src/parsing/FileParser.cpp ../src/parsing/FileParserFactory.cpp ../src/parsing/Triple.cpp ../src/parsing/TSVFileParser.cpp ../src/parsing/ParsingStats.cpp ../src/provenance/ProvenanceGraph.cpp ../src/provenance/ProvenanceGraphFactory.cpp ../src/provenance/ProvenanceSubgraph.cpp ../src/provenance/ProvenanceObject.cpp ../src/provenance/Activity.cpp ../src/provenance/Entity.cpp ../src/provenance/Agent.cpp ../src/provenance/IRIBuilder.cpp ../src/provenance/PROVO.cpp ../src/provenance/RDF.cpp ../src/provenance/RDFS.cpp ../src/provenance/Vocabulary.cpp ../src/provenance/FOAF.cpp ../src/provenance/XSD.cpp  ../src/datatypes/DataValueBuilder.cpp ../src/datatypes/DataValue.cpp ../src/datatypes/DataType.cpp
AM_CPPFLAGS = $(BOOST_CPPFLAGS)
AM_LDFLAGS = $(BOOST_LDFLAGS)
