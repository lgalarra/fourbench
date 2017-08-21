dnl Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT(4Bench, 0.1)


AC_CANONICAL_SYSTEM
AM_INIT_AUTOMAKE([-Wall -Werror foreign subdir-objects -Wno-portability])

AC_PROG_CXX

AC_CONFIG_MACRO_DIR([m4])

AX_CXX_COMPILE_STDCXX_11

#AC_CHECK_LIB([rdf],[librdf_new_world],[AC_MSG_NOTICE(Redland is available)], [AC_MSG_ERROR([Redland library is not installed])])

AX_BOOST_BASE([1.32],,[AC_MSG_ERROR([4Bench needs Boost, but it was not found in your system])])
AX_BOOST_PROGRAM_OPTIONS
AX_BOOST_UNIT_TEST_FRAMEWORK
AC_CONFIG_FILES(Makefile src/Makefile test/Makefile)

AC_OUTPUT

