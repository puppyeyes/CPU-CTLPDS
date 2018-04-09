AM_CFLAGS = $(GMP_CFLAGS) $(ATLAS_CFLAGS)

EXTERNLIB = $(GMP_LIBS) $(ATLAS_LIBS) 

include_HEADERS = iml.h

noinst_HEADERS = basisop.h \
		 certsolve.h \
		 common.h \
		 error.h \
		 latreduce.h \
		 mtrans.h \
		 nonsysolve.h \
		 padiclift.h \
		 reconsolu.h \
		 RNSop.h \
		 nullspace.h

lib_LTLIBRARIES = libiml.la
libiml_la_SOURCES = basisop.c \
	            certsolve.c \
	            error.c \
	            latreduce.c \
	            memalloc.c \
	            mtrans.c \
	   	    nonsysolve.c \
		    padiclift.c \
		    reconsolu.c \
		    RNSop.c \
	            nullspace.c

libiml_la_CFLAGS = $(AM_CFLAGS)
libiml_la_LIBADD = $(EXTERNLIB) \
	 	   $(top_builddir)/repl/librepl.la
libiml_la_LDFLAGS = -version-info 1:0:1
