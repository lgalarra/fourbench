bin_PROGRAMS=4Bench
4Bench_SOURCES=4Bench.cpp utils/string.cpp conf/Conf.cpp parsing/FileParser.cpp parsing/FileParserFactory.cpp \
 parsing/Triple.cpp parsing/TSVFileParser.cpp parsing/ParsingStats.cpp \
 provenance/ProvenanceGraph.cpp provenance/ProvenanceGraphBuilder.cpp provenance/ProvenanceSubgraph.cpp
4Bench_CPPFLAGS=-I$(top_srcdir)/include
AM_CPPFLAGS=$(BOOST_CPPFLAGS)
AM_LDFLAGS=$(BOOST_LDFLAGS)
4Bench_LDADD=$(BOOST_PROGRAM_OPTIONS_LIB)