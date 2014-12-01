
CC=g++ -std=gnu++0x
OPT=-O3
DEBUG_FLAGS=-g


all:	train classify

train:	train.cpp Ngram.o
	${CC} ${OPT} ${DEBUG_FLAGS} Ngram.o train.cpp -o train

classify:	classify.cpp Ngram.o
	${CC} ${OPT} ${DEBUG_FLAGS} Ngram.o classify.cpp -o classify

Ngram.o:	Ngram.h Ngram.cpp
	${CC} ${OPT} ${DEBUG_FLAGS} -c Ngram.cpp -o Ngram.o

clean:	
	rm -rf train classify Ngram.o
