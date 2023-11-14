# dlang-bench
A collection of programs for benchmarking the D language and its compilers

## What is this?
This repository contains a selection of programs to evaluate the D language and its compilers. It was created as part of my bachelor's thesis. The programs have been implemented in D as well as in C/C++ and can work in parallel.

The following benchmarks have been implemented:
- Matrix multiplication using a 1D array (long vs. double, parallel-for vs threading)
- Matrix multiplication using a 2D array (long vs. double, parallel-for vs threading)
- Pi (π) approximation
- Sorting using Radixsort (no count padding vs. count padding)
- Sorting using Quicksort (complete recursion vs. sorting sequentially, if < 100 elements)

The programs use the time-tracker library to measure the runtime of the function calls.

## How do I use it?

### Prerequisites
The compilers `gcc`, `gdc` and `g++` are used for compiling. The Makefiles expect a dlang installation in the home directory. For these to work, the variables `DLANG`, `DLANG_DMD` and `DLANG_LDC` must be adjusted.

### Building
Use `make all` to build the binaries. With `make clean` the binaries are cleaned up.
