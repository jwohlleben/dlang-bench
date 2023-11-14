BIN = bin
INC = include
LIB = lib

DLANG = ~/dlang
DLANG_DMD = $(DLANG)/dmd-2.101.2/activate
DLANG_LDC = $(DLANG)/ldc-1.30.0/activate

C_SRC = src/c-pi
D_SRC = src/d-pi

.PHONY: all
all: source-optimized-gcc \
	 source-optimized-dmd \
	 source-optimized-gdc \
	 source-optimized-ldc \
	 source-optimized-dmd-no-gc \
	 source-optimized-gdc-no-gc \
	 source-optimized-ldc-no-gc \

source-optimized-gcc:
	gcc -Wall -pthread -I$(INC) -L$(LIB) \
		-O3 -march=native \
		$(C_SRC)/pi.c \
		-lttracker \
		-o $(BIN)/optimized_gcc_pi

source-optimized-dmd:
	. $(DLANG_DMD); \
	dmd -I=$(INC) \
		-O -mcpu=native -release -inline -boundscheck=off \
		$(D_SRC)/pi.d \
		$(LIB)/libttracker.a \
		-of=$(BIN)/optimized_dmd_pi

source-optimized-gdc:
	gdc \
		-O3 -march=native -frelease \
		$(D_SRC)/pi.d \
		$(INC)/cttracker.d \
		$(LIB)/libttracker.a \
		-o $(BIN)/optimized_gdc_pi

source-optimized-ldc:
	. $(DLANG_LDC); \
	ldc2 -I=$(INC) \
		--O3 --mcpu=native --release \
		$(D_SRC)/pi.d \
		$(LIB)/libttracker.a \
		-of=$(BIN)/optimized_ldc_pi

source-optimized-dmd-no-gc:
	. $(DLANG_DMD); \
	dmd -version=NO_GC -I=$(INC) \
		-O -mcpu=native -release -inline -boundscheck=off \
		$(D_SRC)/pi.d \
		$(LIB)/libttracker.a \
		-of=$(BIN)/optimized_dmd_no_gc_pi

source-optimized-gdc-no-gc:
	gdc -fversion=NO_GC \
		-O3 -march=native -frelease \
		$(D_SRC)/pi.d \
		$(INC)/cttracker.d \
		$(LIB)/libttracker.a \
		-o $(BIN)/optimized_gdc_no_gc_pi

source-optimized-ldc-no-gc:
	. $(DLANG_LDC); \
	ldc2 -d-version=NO_GC -I=$(INC) \
		--O3 --mcpu=native --release \
		$(D_SRC)/pi.d \
		$(LIB)/libttracker.a \
		-of=$(BIN)/optimized_ldc_no_gc_pi

.PHONY: clean
clean:
	rm -f ./$(BIN)/*

.PHONY: clean-objects
clean-objects:
	rm -f ./$(BIN)/*.o
