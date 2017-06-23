dnl Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT(4Bench, 0.1)


AC_CANONICAL_SYSTEM
AM_INIT_AUTOMAKE([-Wall -Werror foreign subdir-objects])

AC_PROG_CXX

AC_CONFIG_MACRO_DIR([m4])

AX_CXX_COMPILE_STDCXX_11

AX_BOOST_BASE([1.32],,[AC_MSG_ERROR([4Bench needs Boost, but it was not found in your system])])
AX_BOOST_PROGRAM_OPTIONS

AC_CONFIG_FILES(Makefile src/Makefile)

AC_OUTPUT

