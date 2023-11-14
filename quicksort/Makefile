BIN = bin
INC = include
LIB = lib

DLANG = ~/dlang
DLANG_DMD = $(DLANG)/dmd-2.101.2/activate
DLANG_LDC = $(DLANG)/ldc-1.30.0/activate

CPP_QUICK1 = src/cpp-quick1
CPP_QUICK2 = src/cpp-quick2
CPP_QUICK3 = src/cpp-quick3
D_QUICK1 = src/d-quick1
D_QUICK2 = src/d-quick2

H_SRC = src/helper

.PHONY: all
all: quick1-optimized-g++ \
     quick1-optimized-dmd \
	 quick1-optimized-gdc \
	 quick1-optimized-ldc \
	 quick1-optimized-dmd-no-gc \
	 quick1-optimized-gdc-no-gc \
	 quick1-optimized-ldc-no-gc \
	 quick2-optimized-g++ \
     quick2-optimized-dmd \
	 quick2-optimized-gdc \
	 quick2-optimized-ldc \
	 quick2-optimized-dmd-no-gc \
	 quick2-optimized-gdc-no-gc \
	 quick2-optimized-ldc-no-gc \
	 helper

quick1-optimized-g++:
	g++ -Wall -I$(INC) -L$(LIB) \
		-O3 -march=native \
		$(CPP_QUICK1)/quick_sort.cpp \
		$(CPP_QUICK1)/file/file_utils.c \
		$(CPP_QUICK1)/sort/sort_utils.cpp \
		$(CPP_QUICK1)/sort/sort.cpp \
		$(CPP_QUICK1)/taskpool/taskpool.cpp \
		-lttracker \
		-o $(BIN)/optimized_g++_quick1

quick1-optimized-dmd:
	. $(DLANG_DMD); \
	dmd -I=$(INC) \
		-O -mcpu=native -release -inline -boundscheck=off \
		$(D_QUICK1)/quick_sort.d \
		$(D_QUICK1)/sort/sort_utils.d \
		$(D_QUICK1)/sort/sort.d \
		$(LIB)/libttracker.a \
		-of=$(BIN)/optimized_dmd_quick1

quick1-optimized-dmd-no-gc:
	. $(DLANG_DMD); \
	dmd -version=NO_GC -I=$(INC) \
		-O -mcpu=native -release -inline -boundscheck=off \
		$(D_QUICK1)/quick_sort.d \
		$(D_QUICK1)/sort/sort_utils.d \
		$(D_QUICK1)/sort/sort.d \
		$(LIB)/libttracker.a \
		-of=$(BIN)/optimized_dmd_no_gc_quick1

quick1-optimized-gdc:
	gdc \
		-O3 -march=native -frelease \
		$(D_QUICK1)/quick_sort.d \
		$(D_QUICK1)/sort/sort_utils.d \
		$(D_QUICK1)/sort/sort.d \
		$(INC)/cttracker.d \
		$(LIB)/libttracker.a \
		-o $(BIN)/optimized_gdc_quick1

quick1-optimized-gdc-no-gc:
	gdc -fversion=NO_GC \
		-O3 -march=native -frelease \
		$(D_QUICK1)/quick_sort.d \
		$(D_QUICK1)/sort/sort_utils.d \
		$(D_QUICK1)/sort/sort.d \
		$(INC)/cttracker.d \
		$(LIB)/libttracker.a \
		-o $(BIN)/optimized_gdc_no_gc_quick1

quick1-optimized-ldc:
	. $(DLANG_LDC); \
	ldc2 -I=$(INC) \
		--O3 --mcpu=native --release \
		$(D_QUICK1)/quick_sort.d \
		$(D_QUICK1)/sort/sort_utils.d \
		$(D_QUICK1)/sort/sort.d \
		$(LIB)/libttracker.a \
		-of=$(BIN)/optimized_ldc_quick1

quick1-optimized-ldc-no-gc:
	. $(DLANG_LDC); \
	ldc2 -d-version=NO_GC -I=$(INC) \
		--O3 --mcpu=native --release \
		$(D_QUICK1)/quick_sort.d \
		$(D_QUICK1)/sort/sort_utils.d \
		$(D_QUICK1)/sort/sort.d \
		$(LIB)/libttracker.a \
		-of=$(BIN)/optimized_ldc_no_gc_quick1

quick2-optimized-g++:
	g++ -Wall -I$(INC) -L$(LIB) \
		-O3 -march=native \
		$(CPP_QUICK2)/quick_sort.cpp \
		$(CPP_QUICK2)/file/file_utils.c \
		$(CPP_QUICK2)/sort/sort_utils.cpp \
		$(CPP_QUICK2)/sort/sort.cpp \
		$(CPP_QUICK2)/taskpool/taskpool.cpp \
		-lttracker \
		-o $(BIN)/optimized_g++_quick2

quick2-optimized-dmd:
	. $(DLANG_DMD); \
	dmd -I=$(INC) \
		-O -mcpu=native -release -inline -boundscheck=off \
		$(D_QUICK2)/quick_sort.d \
		$(D_QUICK2)/sort/sort_utils.d \
		$(D_QUICK2)/sort/sort.d \
		$(LIB)/libttracker.a \
		-of=$(BIN)/optimized_dmd_quick2

quick2-optimized-dmd-no-gc:
	. $(DLANG_DMD); \
	dmd -version=NO_GC -I=$(INC) \
		-O -mcpu=native -release -inline -boundscheck=off \
		$(D_QUICK2)/quick_sort.d \
		$(D_QUICK2)/sort/sort_utils.d \
		$(D_QUICK2)/sort/sort.d \
		$(LIB)/libttracker.a \
		-of=$(BIN)/optimized_dmd_no_gc_quick2

quick2-optimized-gdc:
	gdc \
		-O3 -march=native -frelease \
		$(D_QUICK2)/quick_sort.d \
		$(D_QUICK2)/sort/sort_utils.d \
		$(D_QUICK2)/sort/sort.d \
		$(INC)/cttracker.d \
		$(LIB)/libttracker.a \
		-o $(BIN)/optimized_gdc_quick2

quick2-optimized-gdc-no-gc:
	gdc -fversion=NO_GC \
		-O3 -march=native -frelease \
		$(D_QUICK2)/quick_sort.d \
		$(D_QUICK2)/sort/sort_utils.d \
		$(D_QUICK2)/sort/sort.d \
		$(INC)/cttracker.d \
		$(LIB)/libttracker.a \
		-o $(BIN)/optimized_gdc_no_gc_quick2

quick2-optimized-ldc:
	. $(DLANG_LDC); \
	ldc2 -I=$(INC) \
		--O3 --mcpu=native --release \
		$(D_QUICK2)/quick_sort.d \
		$(D_QUICK2)/sort/sort_utils.d \
		$(D_QUICK2)/sort/sort.d \
		$(LIB)/libttracker.a \
		-of=$(BIN)/optimized_ldc_quick2

quick2-optimized-ldc-no-gc:
	. $(DLANG_LDC); \
	ldc2 -d-version=NO_GC -I=$(INC) \
		--O3 --mcpu=native --release \
		$(D_QUICK2)/quick_sort.d \
		$(D_QUICK2)/sort/sort_utils.d \
		$(D_QUICK2)/sort/sort.d \
		$(LIB)/libttracker.a \
		-of=$(BIN)/optimized_ldc_no_gc_quick2

quick3-optimized-g++:
	g++ -std=c++20 -Wall -I$(INC) -L$(LIB) \
		-O3 -march=native \
		$(CPP_QUICK3)/quick_sort.cpp \
		$(CPP_QUICK3)/file/file_utils.c \
		$(CPP_QUICK3)/sort/sort_utils.cpp \
		$(CPP_QUICK3)/sort/sort.cpp \
		$(CPP_QUICK3)/taskpool/taskpool.cpp \
		$(CPP_QUICK3)/taskpool/qs_task.cpp \
		-lttracker \
		-o $(BIN)/optimized_g++_quick3

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
