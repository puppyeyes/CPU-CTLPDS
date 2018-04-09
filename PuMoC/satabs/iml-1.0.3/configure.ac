dnl Autoconf

AC_PREREQ(2.59)
AC_INIT(IML, 1.0.3)
AC_CONFIG_AUX_DIR(config)
AM_CONFIG_HEADER(config.h:config-h.in)
AC_CONFIG_SRCDIR([src/RNSop.c])
AM_INIT_AUTOMAKE

# Adds special linker flag for icc
case "x$CC" in
      x*icc) LDFLAGS="$LDFLAGS -lm -no_cpprt" 
	     ;;
      *)     LDFLAGS="$LDFLAGS -lm"
	     ;;
esac

# Checks for programs.
AC_PROG_CC

# Checks for libraries.


# Checks for header files.
AC_HEADER_STDC
AC_DISABLE_SHARED
AM_PROG_LIBTOOL

DEFAULT_CFLAGS="-O3 -Wall"
${CFLAGS:=$DEFAULT_CFLAGS}
CFLAGS="-I. ${CFLAGS}"

AC_CHECK_HEADERS([stdlib.h time.h])

AC_CHECK_HEADERS([math.h],, [echo 'error: could not find required header math.h, configuration aborted.'  exit 1])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_REALLOC

AC_CHECK_FUNCS(bzero memset, break)

AC_CHECK_FUNCS(calloc floor)

IML_VERBOSE_MODE
IML_DEFAULT_PATH
IML_CHECK_GMP(,,[AC_MSG_ERROR(
GMP not found! 
GMP version 3.1.1 or greater is required for this library to compile. 
Please make sure GMP is installed and specify the header and libraries 
location with the options --with-gmp-include=<path> and --with-gmp-lib=<path>
respectively when running configure.
)])

IML_CHECK_ATLAS(,,[AC_MSG_ERROR(
ATLAS not found! 
ATLAS version 3.0 or greater is required for this library to compile. Please make sure ATLAS is installed and specify the header and libraries location with the options --with-atlas-include=<path> and --with-atlas-lib=<path> respectively when running configure.
)])

AC_SUBST(LDFLAGS)
AC_SUBST(ac_aux_dir)
	
AC_OUTPUT([
	Makefile
	config/Makefile
	repl/Makefile
	src/Makefile
	doc/Makefile
	examples/Makefile
	tests/Makefile
])		

echo \
"------------------------------------------------------------------------
Configuration:
 
  Source code location:       ${srcdir}
  Compiler:                   ${CC}
  Compiler flags:             ${CFLAGS}
  Linker flags:               ${LDFLAGS}
  Host System Type:           ${host}
  Install path:               ${prefix}
 
  See config.h for further configuration information.
------------------------------------------------------------------------"
