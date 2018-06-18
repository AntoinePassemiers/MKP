NOMEFILE = mkp.c

CHECK = -Winline -Wall

PAR = -lm -std=c99

CFLAGS = -L. -lmkp $(CHECK) $(PAR)
# debug options will be appended when asked by using 'make debug'
# release optimization will be added then command is 'make release'

.PHONY: all vectorized release debug cleanComp clean

all: clean mkp

mkp: buildLib $(NOMEFILE)
	gcc $(NOMEFILE) -o ../$@ $(CFLAGS)

release: CFLAGS += -O3
release: clean mkp

debug: CFLAGS += -g -DDEBUG  -O0
debug:  clean mkp
# clean
# -g      : debug symbols
# -DDEBUG : declare DEBUG macro


# link libraries
buildLib: mkpdata.o utils.o mkpproblem.o mkpsolution.o mkpneighbourhood.o constructive.o perturbative.o k_move.o annealing.o genetic.o
	ar r libmkp.a *.o

mkpdata.o: mkpdata.h 
	gcc $(CFLAGS) $(PAR) -c mkpdata.h

utils.o: utils.h utils.c
	gcc $(CFLAGS) $(PAR) -c utils.c

mkpproblem.o: mkpproblem.h mkpproblem.c
	gcc $(CFLAGS) $(PAR) -c mkpproblem.c

mkpsolution.o: mkpsolution.h mkpsolution.c
	gcc $(CFLAGS) $(PAR) -c mkpsolution.c

mkpneighbourhood.o: mkpneighbourhood.h mkpneighbourhood.c
	gcc $(CFLAGS) $(PAR) -c mkpneighbourhood.c

constructive.o: constructive.h constructive.c
	gcc $(CFLAGS) $(PAR) -c constructive.c

perturbative.o: perturbative.h perturbative.c
	gcc $(CFLAGS) $(PAR) -c perturbative.c 

k_move.o: k_move.h k_move.c
	gcc $(CFLAGS) $(PAR) -c k_move.c 

genetic.o: genetic.h genetic.c
	gcc $(CFLAGS) $(PAR) -c genetic.c

annealing.o: annealing.h annealing.c
	gcc $(CFLAGS) $(PAR) -c annealing.c 

cleanComp:
	clear
	-rm mkp
	-rm libmkp.a

clean:
	clear
	-rm mkp
	-rm libmkp.a
	-rm *.o
	-rm *.gch
