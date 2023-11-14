BIN = bin
INC = include
LIB = lib

DLANG = ~/dlang
DLANG_DMD = $(DLANG)/dmd-2.101.2/activate
DLANG_LDC = $(DLANG)/ldc-1.30.0/activate

C_RADIX1 = src/c-radix1
D_RADIX1 = src/d-radix1
C_RADIX2 = src/c-radix2
D_RADIX2 = src/d-radix2

H_SRC = src/helper

.PHONY: all
all: radix1-optimized-gcc \
	 radix1-optimized-dmd \
	 radix1-optimized-gdc \
	 radix1-optimized-ldc \
	 radix1-optimized-dmd-no-gc \
	 radix1-optimized-gdc-no-gc \
	 radix1-optimized-ldc-no-gc \
	 radix2-optimized-gcc \
	 radix2-optimized-dmd \
	 radix2-optimized-gdc \
	 radix2-optimized-ldc \
	 radix2-optimized-dmd-no-gc \
	 radix2-optimized-gdc-no-gc \
	 radix2-optimized-ldc-no-gc \
	 helper

radix1-optimized-gcc:
	gcc -Wall -pthread -I$(INC) -L$(LIB) \
		-O3 -march=native \
		$(C_RADIX1)/radix_sort.c \
		$(C_RADIX1)/file/file_utils.c \
		$(C_RADIX1)/sort/sort_utils.c \
		$(C_RADIX1)/sort/sort.c \
		-lttracker \
		-o $(BIN)/optimized_gcc_radix1

radix1-optimized-dmd:
	. $(DLANG_DMD); \
	dmd -I=$(INC) \
		-O -mcpu=native -release -inline -boundscheck=off \
		$(D_RADIX1)/radix_sort.d \
		$(D_RADIX1)/sort/sort_utils.d \
		$(D_RADIX1)/sort/sort.d \
		$(LIB)/libttracker.a \
		-of=$(BIN)/optimized_dmd_radix1

radix1-optimized-dmd-no-gc:
	. $(DLANG_DMD); \
	dmd -version=NO_GC -I=$(INC) \
		-O -mcpu=native -release -inline -boundscheck=off \
		$(D_RADIX1)/radix_sort.d \
		$(D_RADIX1)/sort/sort_utils.d \
		$(D_RADIX1)/sort/sort.d \
		$(LIB)/libttracker.a \
		-of=$(BIN)/optimized_dmd_no_gc_radix1

radix1-optimized-gdc:
	gdc \
		-O3 -march=native -frelease \
		$(D_RADIX1)/radix_sort.d \
		$(D_RADIX1)/sort/sort_utils.d \
		$(D_RADIX1)/sort/sort.d \
		$(INC)/cttracker.d \
		$(LIB)/libttracker.a \
		-o $(BIN)/optimized_gdc_radix1

radix1-optimized-gdc-no-gc:
	gdc -fversion=NO_GC \
		-O3 -march=native -frelease \
		$(D_RADIX1)/radix_sort.d \
		$(D_RADIX1)/sort/sort_utils.d \
		$(D_RADIX1)/sort/sort.d \
		$(INC)/cttracker.d \
		$(LIB)/libttracker.a \
		-o $(BIN)/optimized_gdc_no_gc_radix1

radix1-optimized-ldc:
	. $(DLANG_LDC); \
	ldc2 -I=$(INC) \
		--O3 --mcpu=native --release \
		$(D_RADIX1)/radix_sort.d \
		$(D_RADIX1)/sort/sort_utils.d \
		$(D_RADIX1)/sort/sort.d \
		$(LIB)/libttracker.a \
		-of=$(BIN)/optimized_ldc_radix1

radix1-optimized-ldc-no-gc:
	. $(DLANG_LDC); \
	ldc2 -d-version=NO_GC -I=$(INC) \
		--O3 --mcpu=native --release \
		$(D_RADIX1)/radix_sort.d \
		$(D_RADIX1)/sort/sort_utils.d \
		$(D_RADIX1)/sort/sort.d \
		$(LIB)/libttracker.a \
		-of=$(BIN)/optimized_ldc_no_gc_radix1

radix2-optimized-gcc:
	gcc -Wall -pthread -I$(INC) -L$(LIB) \
		-O3 -march=native \
		$(C_RADIX2)/radix_sort.c \
		$(C_RADIX2)/file/file_utils.c \
		$(C_RADIX2)/sort/sort_utils.c \
		$(C_RADIX2)/sort/sort.c \
		-lttracker \
		-o $(BIN)/optimized_gcc_radix2

radix2-optimized-dmd:
	. $(DLANG_DMD); \
	dmd -I=$(INC) \
		-O -mcpu=native -release -inline -boundscheck=off \
		$(D_RADIX2)/radix_sort.d \
		$(D_RADIX2)/sort/sort_utils.d \
		$(D_RADIX2)/sort/sort.d \
		$(LIB)/libttracker.a \
		-of=$(BIN)/optimized_dmd_radix2

radix2-optimized-dmd-no-gc:
	. $(DLANG_DMD); \
	dmd -version=NO_GC -I=$(INC) \
		-O -mcpu=native -release -inline -boundscheck=off \
		$(D_RADIX2)/radix_sort.d \
		$(D_RADIX2)/sort/sort_utils.d \
		$(D_RADIX2)/sort/sort.d \
		$(LIB)/libttracker.a \
		-of=$(BIN)/optimized_dmd_no_gc_radix2

radix2-optimized-gdc:
	gdc \
		-O3 -march=native -frelease \
		$(D_RADIX2)/radix_sort.d \
		$(D_RADIX2)/sort/sort_utils.d \
		$(D_RADIX2)/sort/sort.d \
		$(INC)/cttracker.d \
		$(LIB)/libttracker.a \
		-o $(BIN)/optimized_gdc_radix2

radix2-optimized-gdc-no-gc:
	gdc -fversion=NO_GC \
		-O3 -march=native -frelease \
		$(D_RADIX2)/radix_sort.d \
		$(D_RADIX2)/sort/sort_utils.d \
		$(D_RADIX2)/sort/sort.d \
		$(INC)/cttracker.d \
		$(LIB)/libttracker.a \
		-o $(BIN)/optimized_gdc_no_gc_radix2

radix2-optimized-ldc:
	. $(DLANG_LDC); \
	ldc2 -I=$(INC) \
		--O3 --mcpu=native --release \
		$(D_RADIX2)/radix_sort.d \
		$(D_RADIX2)/sort/sort_utils.d \
		$(D_RADIX2)/sort/sort.d \
		$(LIB)/libttracker.a \
		-of=$(BIN)/optimized_ldc_radix2

radix2-optimized-ldc-no-gc:
	. $(DLANG_LDC); \
	ldc2 -d-version=NO_GC -I=$(INC) \
		--O3 --mcpu=native --release \
		$(D_RADIX2)/radix_sort.d \
		$(D_RADIX2)/sort/sort_utils.d \
		$(D_RADIX2)/sort/sort.d \
		$(LIB)/libttracker.a \
		-of=$(BIN)/optimized_ldc_no_gc_radix2

helper:
	gcc -Wall \
		$(H_SRC)/sort_create_array.c \
		-o $(BIN)/create_array

.PHONY: clean
clean:
	rm -f ./$(BIN)/*

.PHONY: clean-objects
clean-objects:
	rm -f ./$(BIN)/*.o
