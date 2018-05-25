util.o: cudd-2.4.1/include/util.h
cudd.o: cudd-2.4.1/include/cudd.h cudd-2.4.1/include/mtr.h \
 cudd-2.4.1/include/epd.h
mtr.o: cudd-2.4.1/include/mtr.h
epd.o: cudd-2.4.1/include/epd.h
name.o: name.h bdd.h cudd-2.4.1/include/util.h cudd-2.4.1/include/cudd.h \
 cudd-2.4.1/include/mtr.h cudd-2.4.1/include/epd.h name.h
reorder.o: reorder.h variables.h name.h bdd.h cudd-2.4.1/include/util.h \
 cudd-2.4.1/include/cudd.h cudd-2.4.1/include/mtr.h \
 cudd-2.4.1/include/epd.h
variables.o: variables.h name.h bdd.h cudd-2.4.1/include/util.h \
 cudd-2.4.1/include/cudd.h cudd-2.4.1/include/mtr.h \
 cudd-2.4.1/include/epd.h
bpstruc.o: bpstruc.h name.h bdd.h cudd-2.4.1/include/util.h \
 cudd-2.4.1/include/cudd.h cudd-2.4.1/include/mtr.h \
 cudd-2.4.1/include/epd.h
common.o: common.h
sort.o: sort.h
cuddInt.o: cudd-2.4.1/include/cuddInt.h cudd-2.4.1/include/cudd.h \
 cudd-2.4.1/include/mtr.h cudd-2.4.1/include/epd.h \
 cudd-2.4.1/include/st.h
st.o: cudd-2.4.1/include/st.h
