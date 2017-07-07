bin_PROGRAMS = TSVFileParserTest ProvenanceGraphTest
TSVFileParserTest_SOURCES = parsing/TSVFileParserTest.cpp ../src/conf/Conf.cpp ../src/utils/string.cpp ../src/parsing/FileParser.cpp ../src/parsing/FileParserFactory.cpp ../src/parsing/Triple.cpp ../src/parsing/TSVFileParser.cpp ../src/parsing/ParsingStats.cpp
TSVFileParserTest_CPPFLAGS = -I$(top_srcdir)/include
TSVFileParserTest_LDADD = $(BOOST_PROGRAM_OPTIONS_LIB)

ProvenanceGraphTest_CPPFLAGS = $(TSVFileParserTest_CPPFLAGS)
ProvenanceGraphTest_LDADD = $(TSVFileParserTest_LDADD)
ProvenanceGraphTest_SOURCES = provenance/ProvenanceGraphTest.cpp ../src/conf/Conf.cpp ../src/utils/string.cpp ../src/parsing/FileParser.cpp ../src/parsing/FileParserFactory.cpp ../src/parsing/Triple.cpp ../src/parsing/TSVFileParser.cpp ../src/parsing/ParsingStats.cpp ../src/provenance/ProvenanceGraph.cpp ../src/provenance/ProvenanceGraphBuilder.cpp ../src/provenance/ProvenanceSubgraph.cpp 
AM_CPPFLAGS = $(BOOST_CPPFLAGS)
AM_LDFLAGS = $(BOOST_LDFLAGS)
