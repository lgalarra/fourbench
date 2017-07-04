bin_PROGRAMS=TSVFileParserTest
TSVFileParserTest_SOURCES=parsing/TSVFileParserTest.cpp ../src/conf/Conf.cpp ../src/utils/string.cpp ../src/parsing/FileParser.cpp ../src/parsing/FileParserFactory.cpp ../src/parsing/Triple.cpp ../src/parsing/TSVFileParser.cpp
TSVFileParserTest_CPPFLAGS = -I$(top_srcdir)/include
AM_CPPFLAGS = $(BOOST_CPPFLAGS)
AM_LDFLAGS = $(BOOST_LDFLAGS)
TSVFileParserTest_LDADD = $(BOOST_PROGRAM_OPTIONS_LIB)