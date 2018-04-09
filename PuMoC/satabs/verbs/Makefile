###############################################################################
# 	cFlags= -c -O3 -DNDEBUG -static -fno-strict-aliasing 		#
###############################################################################
cFlags= -c -O3 -funroll-all-loops -I./inc -I.. -I.
###############################################################################
oFiles= arit.o bool.o conv.o form.o issat.o main.o trop.o util.o
cFiles= arit.c bool.c conv.c form.c issat.c main.c trop.c util.c
###############################################################################
verbs:	$(oFiles) config.h
	rm main.o; make verbss
verbss:	$(oFiles) config.h
	gcc -o verbs $(oFiles) -lm
clean:
	touch xtmpx main.o; rm *.o *tmp*
###############################################################################
.c.o:
	gcc $(cFlags) $<
###############################################################################
