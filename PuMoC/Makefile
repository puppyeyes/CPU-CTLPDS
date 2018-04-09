# Cudd stuff:
CUDDVER =cudd-2.4.1
CUDDINCLUDEDIR=$(CUDDVER)/include
CUDDLIBDIR=$(CUDDVER)/lib
CUDDLIBS=cudd mtr st util epd
PuMoC = PuMoC
CCINCLUDEDIR = ""
CC = gcc -pg -g -O3 -Wall  -I$(CUDDINCLUDEDIR) #-DYYDEBUG
LINK=$(CC)
LIBS =  -L$(CUDDLIBDIR)  -lm $(patsubst %,-l%,$(CUDDLIBS))
O = o


TMPFILES = baparse.c balex.c pdsparse.c pdslex.c \
	   bp.c bplex.c ltlparse.c ltllex.c prop.c proplex.c \
	   ctlparse.c ctllex.c abpdslex.c abpdsparse.c regexplex.c regexpparse.c 

MODULES = ba baparse common fatrans ftree heads main mcheck \
	  name pds pdsparse poststar prestar prereach reorder bdd \
	  graph elim closure xb el cycle bktrace fwtrace trace \
	  bp bptree bpmake bpgraph bptrace bpuse sort netconv \
	  pa ltlparse prop expr ctlparse afa abpds abpdsprestar \
	  abpdsparse regexpparse nfa

OBJECTS = $(MODULES:=.$(O))

$(PuMoC): $(OBJECTS)
ifdef WIN32
	$(LINK) $(OBJECTS) /OUT:$@ $(LIBS)
else # POSIX
	$(LINK) $(OBJECTS) -o $@ $(LIBS)
endif

# Compiling:
%.$(O): %.c
	$(CC) -c $<

# miscellaneous parsers

baparse.$(O): baparse.c balex.c
baparse.c: baparse.y
	bison baparse.y -p ba -o $@
balex.c: balex.l
	flex -o$@ -Pba balex.l

pdsparse.$(O): pdsparse.c pdslex.c
pdsparse.c: pdsparse.y
	bison pdsparse.y -p pds -o $@
pdslex.c: pdslex.l
	flex -o$@ -Ppds pdslex.l

abpdsparse.$(O): abpdsparse.c abpdslex.c
abpdsparse.c: abpdsparse.y
	bison abpdsparse.y -p abpds -o $@
abpdslex.c: abpdslex.l
	flex -o$@ -Pabpds abpdslex.l

bp.$(O): bp.c bplex.c
bp.c: bp.y
	bison bp.y -p bp -o $@
bplex.c: bplex.l
	flex -o$@ -Pbp bplex.l

ltlparse.$(O): ltlparse.c ltllex.c
ltlparse.c: ltlparse.y ltllex.c
	bison ltlparse.y -p ltl -o $@
ltllex.c: ltllex.l
	flex -t -Pltl ltllex.l | \
	sed 's/getc( ltlin )) != EOF/*ltlstr) \&\& (ltlstr += !!*ltlstr)/g' > $@
	
	
	
regexpparse.$(O): regexpparse.c regexplex.c
regexpparse.c: regexpparse.y regexplex.c
	bison regexpparse.y -p regexp -o $@
regexplex.c: regexplex.l
	flex -t -Pregexp regexplex.l | \
	sed 's/getc( regexpin )) != EOF/*regexpstr) \&\& (regexpstr += !!*regexpstr)/g' > $@
	
	


ctlparse.$(O): ctlparse.c ctllex.c
ctlparse.c: ctlparse.y ctllex.c
	bison ctlparse.y -p ctl -o $@
ctllex.c: ctllex.l
	flex -o$@ -Pctl ctllex.l


prop.$(O): prop.c proplex.c
prop.c: prop.y proplex.c
	bison prop.y -p prop -o $@
proplex.c: proplex.l
	flex -t -Pprop proplex.l | \
	sed 's/getc( propin )) != EOF/*propstr) \&\& (propstr += !!*propstr)/g'>$@

# clean

clean: 
	rm -f $(PuMoC) $(OBJECTS) $(TMPFILES) \
              frag_* core* *.$(O) *.output mon.out gmon.out *.d .deps

# Dependencies

%.d: %.c
	$(SHELL) -ec '$(CC) -MM $? | sed '\''s/$*.o/& $@/g'\'' > $@'

DEPS = $(OBJECTS:%.$(O)=%.d)

.deps: $(DEPS)
	echo " " $(DEPS) | \
	sed 's/[ 	][ 	]*/#include /g' | tr '#' '\012' > .deps

include .deps

