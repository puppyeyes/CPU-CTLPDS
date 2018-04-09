###############################################################################
#cFlags= -c -O3 -DNDEBUG -static -fno-strict-aliasing 
###############################################################################
cFlags= -c -O3 -funroll-all-loops -w -I. -I./inc
###############################################################################
oFiles= q1rd.o q2nt.o qbf.o bp2q.o bp2s.o tbd.o dmc.o f2p.o form.o issat.o \
	main.o opt1.o p2bp.o tree.o prep.o 
cFiles= q1rd.c q2nt.c qbf.c bp2q.c bp2s.c tbd.c dmc.c f2p.c form.c issat.c \
	main.c opt1.c p2bp.c tree.c prep.c 
###############################################################################
verds:	$(oFiles)
	gcc -o verds $(oFiles) -lm
###############################################################################
.c.o:	config.h
	gcc $(cFlags) $<
###############################################################################
