BIN = bin
INC = include
LIB = lib

DLANG = ~/dlang
DLANG_DMD = $(DLANG)/dmd-2.101.2/activate
DLANG_LDC = $(DLANG)/ldc-1.30.0/activate

C_L_SRC = src/c-long
C_D_SRC = src/c-double

D_L_SRC = src/d-long
D_D_SRC = src/d-double
D_L_PF_SRC = src/d-long-pf
D_D_PF_SRC = src/d-double-pf

H_SRC = src/helper

.PHONY: all
all: source-optimized-gcc \
	 source-optimized-dmd \
	 source-optimized-gdc \
	 source-optimized-ldc \
	 source-optimized-dmd-no-gc \
	 source-optimized-gdc-no-gc \
	 source-optimized-ldc-no-gc \
	 source-helper

source-optimized-gcc:
	gcc -Wall -pthread -I$(INC) -L$(LIB) \
		-O3 -march=native \
		$(C_L_SRC)/matrix_mult.c \
		$(C_L_SRC)/file/file_utils.c \
		$(C_L_SRC)/matrix/matrix_utils.c \
		$(C_L_SRC)/matrix/matrix.c \
		-lttracker \
		-o $(BIN)/optimized_gcc_long

	gcc -Wall -pthread -I$(INC) -L$(LIB) \
		-O3 -march=native \
		$(C_D_SRC)/matrix_mult.c \
		$(C_D_SRC)/file/file_utils.c \
		$(C_D_SRC)/matrix/matrix_utils.c \
		$(C_D_SRC)/matrix/matrix.c \
		-lttracker \
		-o $(BIN)/optimized_gcc_double

source-optimized-dmd:
	. $(DLANG_DMD); \
	dmd -I=$(INC) \
		-O -mcpu=native -release -inline -boundscheck=off \
		$(D_L_SRC)/matrix_mult.d \
		$(D_L_SRC)/matrix/matrix_utils.d \
		$(D_L_SRC)/matrix/matrix.d \
		$(LIB)/libttracker.a \
		-of=$(BIN)/optimized_dmd_long

	. $(DLANG_DMD); \
	dmd -I=$(INC) \
		-O -mcpu=native -release -inline -boundscheck=off \
		$(D_L_PF_SRC)/matrix_mult.d \
		$(D_L_PF_SRC)/matrix/matrix_utils.d \
		$(D_L_PF_SRC)/matrix/matrix.d \
		$(LIB)/libttracker.a \
		-of=$(BIN)/optimized_dmd_long_pf

	. $(DLANG_DMD); \
	dmd -I=$(INC) \
		-O -mcpu=native -release -inline -boundscheck=off \
		$(D_D_SRC)/matrix_mult.d \
		$(D_D_SRC)/matrix/matrix_utils.d \
		$(D_D_SRC)/matrix/matrix.d \
		$(LIB)/libttracker.a \
		-of=$(BIN)/optimized_dmd_double

	. $(DLANG_DMD); \
	dmd -I=$(INC) \
		-O -mcpu=native -release -inline -boundscheck=off \
		$(D_D_PF_SRC)/matrix_mult.d \
		$(D_D_PF_SRC)/matrix/matrix_utils.d \
		$(D_D_PF_SRC)/matrix/matrix.d \
		$(LIB)/libttracker.a \
		-of=$(BIN)/optimized_dmd_double_pf

source-optimized-gdc:
	gdc \
		-O3 -march=native -frelease \
		$(D_L_SRC)/matrix_mult.d \
		$(D_L_SRC)/matrix/matrix_utils.d \
		$(D_L_SRC)/matrix/matrix.d \
		$(INC)/cttracker.d \
		$(LIB)/libttracker.a \
		-o $(BIN)/optimized_gdc_long

	gdc \
		-O3 -march=native -frelease \
		$(D_L_PF_SRC)/matrix_mult.d \
		$(D_L_PF_SRC)/matrix/matrix_utils.d \
		$(D_L_PF_SRC)/matrix/matrix.d \
		$(INC)/cttracker.d \
		$(LIB)/libttracker.a \
		-o $(BIN)/optimized_gdc_long_pf

	gdc \
		-O3 -march=native -frelease \
		$(D_D_SRC)/matrix_mult.d \
		$(D_D_SRC)/matrix/matrix_utils.d \
		$(D_D_SRC)/matrix/matrix.d \
		$(INC)/cttracker.d \
		$(LIB)/libttracker.a \
		-o $(BIN)/optimized_gdc_double

	gdc \
		-O3 -march=native -frelease \
		$(D_D_PF_SRC)/matrix_mult.d \
		$(D_D_PF_SRC)/matrix/matrix_utils.d \
		$(D_D_PF_SRC)/matrix/matrix.d \
		$(INC)/cttracker.d \
		$(LIB)/libttracker.a \
		-o $(BIN)/optimized_gdc_double_pf

source-optimized-ldc:
	. $(DLANG_LDC); \
	ldc2 -I=$(INC) \
		--O3 --mcpu=native --release \
		$(D_L_SRC)/matrix_mult.d \
		$(D_L_SRC)/matrix/matrix_utils.d \
		$(D_L_SRC)/matrix/matrix.d \
		$(LIB)/libttracker.a \
		-of=$(BIN)/optimized_ldc_long

	. $(DLANG_LDC); \
	ldc2 -I=$(INC) \
		--O3 --mcpu=native --release \
		$(D_L_PF_SRC)/matrix_mult.d \
		$(D_L_PF_SRC)/matrix/matrix_utils.d \
		$(D_L_PF_SRC)/matrix/matrix.d \
		$(LIB)/libttracker.a \
		-of=$(BIN)/optimized_ldc_long_pf

	. $(DLANG_LDC); \
	ldc2 -I=$(INC) \
		--O3 --mcpu=native --release \
		$(D_D_SRC)/matrix_mult.d \
		$(D_D_SRC)/matrix/matrix_utils.d \
		$(D_D_SRC)/matrix/matrix.d \
		$(LIB)/libttracker.a \
		-of=$(BIN)/optimized_ldc_double

	. $(DLANG_LDC); \
	ldc2 -I=$(INC) \
		--O3 --mcpu=native --release \
		$(D_D_PF_SRC)/matrix_mult.d \
		$(D_D_PF_SRC)/matrix/matrix_utils.d \
		$(D_D_PF_SRC)/matrix/matrix.d \
		$(LIB)/libttracker.a \
		-of=$(BIN)/optimized_ldc_double_pf

source-optimized-dmd-no-gc:
	. $(DLANG_DMD); \
	dmd -version=NO_GC -I=$(INC) \
		-O -mcpu=native -release -inline -boundscheck=off \
		$(D_L_SRC)/matrix_mult.d \
		$(D_L_SRC)/matrix/matrix_utils.d \
		$(D_L_SRC)/matrix/matrix.d \
		$(LIB)/libttracker.a \
		-of=$(BIN)/optimized_dmd_no_gc_long

	. $(DLANG_DMD); \
	dmd -version=NO_GC -I=$(INC) \
		-O -mcpu=native -release -inline -boundscheck=off \
		$(D_L_PF_SRC)/matrix_mult.d \
		$(D_L_PF_SRC)/matrix/matrix_utils.d \
		$(D_L_PF_SRC)/matrix/matrix.d \
		$(LIB)/libttracker.a \
		-of=$(BIN)/optimized_dmd_no_gc_long_pf

	. $(DLANG_DMD); \
	dmd -version=NO_GC -I=$(INC) \
		-O -mcpu=native -release -inline -boundscheck=off \
		$(D_D_SRC)/matrix_mult.d \
		$(D_D_SRC)/matrix/matrix_utils.d \
		$(D_D_SRC)/matrix/matrix.d \
		$(LIB)/libttracker.a \
		-of=$(BIN)/optimized_dmd_no_gc_double

	. $(DLANG_DMD); \
	dmd -version=NO_GC -I=$(INC) \
		-O -mcpu=native -release -inline -boundscheck=off \
		$(D_D_PF_SRC)/matrix_mult.d \
		$(D_D_PF_SRC)/matrix/matrix_utils.d \
		$(D_D_PF_SRC)/matrix/matrix.d \
		$(LIB)/libttracker.a \
		-of=$(BIN)/optimized_dmd_no_gc_double_pf

source-optimized-gdc-no-gc:
	gdc -fversion=NO_GC \
		-O3 -march=native -frelease \
		$(D_L_SRC)/matrix_mult.d \
		$(D_L_SRC)/matrix/matrix_utils.d \
		$(D_L_SRC)/matrix/matrix.d \
		$(INC)/cttracker.d \
		$(LIB)/libttracker.a \
		-o $(BIN)/optimized_gdc_no_gc_long

	gdc -fversion=NO_GC \
		-O3 -march=native -frelease \
		$(D_L_PF_SRC)/matrix_mult.d \
		$(D_L_PF_SRC)/matrix/matrix_utils.d \
		$(D_L_PF_SRC)/matrix/matrix.d \
		$(INC)/cttracker.d \
		$(LIB)/libttracker.a \
		-o $(BIN)/optimized_gdc_no_gc_long_pf

	gdc -fversion=NO_GC \
		-O3 -march=native -frelease \
		$(D_D_SRC)/matrix_mult.d \
		$(D_D_SRC)/matrix/matrix_utils.d \
		$(D_D_SRC)/matrix/matrix.d \
		$(INC)/cttracker.d \
		$(LIB)/libttracker.a \
		-o $(BIN)/optimized_gdc_no_gc_double

	gdc -fversion=NO_GC \
		-O3 -march=native -frelease \
		$(D_D_PF_SRC)/matrix_mult.d \
		$(D_D_PF_SRC)/matrix/matrix_utils.d \
		$(D_D_PF_SRC)/matrix/matrix.d \
		$(INC)/cttracker.d \
		$(LIB)/libttracker.a \
		-o $(BIN)/optimized_gdc_no_gc_double_pf

source-optimized-ldc-no-gc:
	. $(DLANG_LDC); \
	ldc2 -d-version=NO_GC -I=$(INC) \
		--O3 --mcpu=native --release \
		$(D_L_SRC)/matrix_mult.d \
		$(D_L_SRC)/matrix/matrix_utils.d \
		$(D_L_SRC)/matrix/matrix.d \
		$(LIB)/libttracker.a \
		-of=$(BIN)/optimized_ldc_no_gc_long

	. $(DLANG_LDC); \
	ldc2 -d-version=NO_GC -I=$(INC) \
		--O3 --mcpu=native --release \
		$(D_L_PF_SRC)/matrix_mult.d \
		$(D_L_PF_SRC)/matrix/matrix_utils.d \
		$(D_L_PF_SRC)/matrix/matrix.d \
		$(LIB)/libttracker.a \
		-of=$(BIN)/optimized_ldc_no_gc_long_pf

	. $(DLANG_LDC); \
	ldc2 -d-version=NO_GC -I=$(INC) \
		--O3 --mcpu=native --release \
		$(D_D_SRC)/matrix_mult.d \
		$(D_D_SRC)/matrix/matrix_utils.d \
		$(D_D_SRC)/matrix/matrix.d \
		$(LIB)/libttracker.a \
		-of=$(BIN)/optimized_ldc_no_gc_double

	. $(DLANG_LDC); \
	ldc2 -d-version=NO_GC -I=$(INC) \
		--O3 --mcpu=native --release \
		$(D_D_PF_SRC)/matrix_mult.d \
		$(D_D_PF_SRC)/matrix/matrix_utils.d \
		$(D_D_PF_SRC)/matrix/matrix.d \
		$(LIB)/libttracker.a \
		-of=$(BIN)/optimized_ldc_no_gc_double_pf

source-helper:
	gcc -Wall \
		$(H_SRC)/matrix_create_long.c \
		-o $(BIN)/create_long

	gcc -Wall \
		$(H_SRC)/matrix_create_double.c \
		-o $(BIN)/create_double

.PHONY: clean
clean:
	rm -f ./$(BIN)/*

.PHONY: clean-objects
clean-objects:
	rm -f ./$(BIN)/*.o
