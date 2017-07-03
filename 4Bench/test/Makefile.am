bin_PROGRAMS=TSVFileParserTest
TSVFileParserTest_SOURCES=$(wildcard parsing/*.cpp) $(wildcard utils/*.cpp)
TSVFileParserTest_CPPFLAGS = -I$(top_srcdir)/include
AM_CPPFLAGS = $(BOOST_CPPFLAGS)
AM_LDFLAGS = $(BOOST_LDFLAGS)
TSVFileParserTest_LDADD = $(BOOST_PROGRAM_OPTIONS_LIB)