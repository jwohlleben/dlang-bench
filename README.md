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

### Matrix Multiplication
To create a matrix, run e.g. `./create_long 3 3 -9 9 matrix`, which creates a 3x3 `matrix` file with values between -9 and 9. In my case the file contains `[[9,0,-2],[-5,-9,5],[-6,-1,-6]]`.

To multiply matrices, run e.g. `./optimized_gcc_long matrix1 matrix2 result 16`, which takes two matrix files `matrix1` and `matrix2`, multiplies them using 16 threads and writes the result in the file `result`.

### Pi Approximation
Run `./optimized_gcc_pi 1000 4` to approximate π with 1000 steps and 4 threads.

### Sorting Algorithms
To create an array for sorting, run `./create_array 10 array`, which creates a file named `array` with 10 elements. In my case the file contains `2286629601,2342179546,3953731515,2715744349,2310085744,738926514,1527599671,352712622,3682162434,1021963721`.

To sort an array, run e.g. `./optimized_gcc_radix2 array 8`, which sorts the array `array` using 8 threads.

### Benchmarking
After a program has finished running, the times for the various segments are output in CSV format. For example, an output could look like this: `7.087640298,0.971018171,6.104621552,0.011341552`. In my programs the first parameter is always the runtime of the `main`-function. The other parameters are used for measuring the time to calculate, sort, verfiy, read or write something.

To automate program execution, the `benchmark.sh` script can be used. It takes a config file, which can look like this:
```
iterations=10
warmup_threads=16
warmup_time_initial=30s
output_dir=out_normal
csv_header=total,calculating
run:
./bin/optimized_gcc_pi 100000000
./bin/optimized_gcc_pi 200000000
./bin/optimized_gcc_pi 300000000
./bin/optimized_gcc_pi 400000000
./bin/optimized_gcc_pi 500000000
./bin/optimized_gcc_pi 600000000
./bin/optimized_gcc_pi 700000000
./bin/optimized_gcc_pi 800000000
./bin/optimized_gcc_pi 900000000
./bin/optimized_gcc_pi 1000000000
```
The package `stress-ng` is required for using the CPU warmup feature.

