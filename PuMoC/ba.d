name.o: name.h bdd.h cudd-2.4.1/include/util.h cudd-2.4.1/include/cudd.h \
 cudd-2.4.1/include/mtr.h cudd-2.4.1/include/epd.h name.h
util.o: cudd-2.4.1/include/util.h
cudd.o: cudd-2.4.1/include/cudd.h cudd-2.4.1/include/mtr.h \
 cudd-2.4.1/include/epd.h
mtr.o: cudd-2.4.1/include/mtr.h
epd.o: cudd-2.4.1/include/epd.h
ftree.o: ftree.h name.h bdd.h cudd-2.4.1/include/util.h \
 cudd-2.4.1/include/cudd.h cudd-2.4.1/include/mtr.h \
 cudd-2.4.1/include/epd.h variables.h
variables.o: variables.h name.h bdd.h cudd-2.4.1/include/util.h \
 cudd-2.4.1/include/cudd.h cudd-2.4.1/include/mtr.h \
 cudd-2.4.1/include/epd.h
common.o: common.h
