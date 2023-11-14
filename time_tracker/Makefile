BIN = bin
SRC = src

all: source

source:
	gcc -Wall -c -g -shared \
		$(SRC)/ttracker.c \
		-o $(BIN)/ttracker.o

	ar rcs $(BIN)/libttracker.a $(BIN)/ttracker.o

.PHONY: clean
clean:
	rm -f ./$(BIN)/*
