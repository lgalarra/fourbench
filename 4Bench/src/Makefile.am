bin_PROGRAMS=4Bench
4Bench_SOURCES=4Bench.cpp $(wildcard conf/*.cpp) $(wildcard parsing/*.cpp)
4Bench_CPPFLAGS = -I$(top_srcdir)/include
AM_CPPFLAGS = $(BOOST_CPPFLAGS)
AM_LDFLAGS = $(BOOST_LDFLAGS)
4Bench_LDADD = $(BOOST_PROGRAM_OPTIONS_LIB)