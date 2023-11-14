module my_sort;

import std.algorithm;
import std.parallelism;

import sort_utils;

/**
 * Exception for parsing errors
 */
class SortParserException : Exception
{
    this (string msg, string file = __FILE__, size_t line = __LINE__)
    {
        super(msg, file, line);
    }
}

/**
 * Sorts the array using
 * Introsort, if threadCount <= 1
 * Quicksort, if threadCount >  1
 *
 * @param array The array to be sorted
 * @param threadCount Thread count
 */
void mySort(uint[] array, uint threadCount)
{
    defaultPoolThreads(threadCount - 1);

    if (threadCount <= 1)
    {
        sort(array);
    }
    else
    {
        sortParallel(array);
    }
}

/**
 * Sorts the array using parallel quicksort
 *
 * Function is based on parallelSort
 * from https://dlang.org/phobos/std_parallelism.html
 *
 * @param array The array to be sorted
 */
void sortParallel(uint[] array)
{
    // Sort small subarrays serially
    if (array.length < 100)
    {
         std.algorithm.sort(array);
         return;
    }

    // Partition the array
    swap(array[$ / 2], array[$ - 1]);
    auto pivot = array[$ - 1];
    bool lessThanPivot(uint elem) { return elem < pivot; }

    auto greaterEqual = partition!lessThanPivot(array[0..$ - 1]);
    swap(array[$ - greaterEqual.length - 1], array[$ - 1]);

    auto less = array[0..$ - greaterEqual.length - 1];
    greaterEqual = array[$ - greaterEqual.length..$];

    // Execute both recursion branches in parallel
    auto recurseTask = task!sortParallel(greaterEqual);
    
    // Put right side in Taskpool
    taskPool.put(recurseTask);
    
    // Work on left side
    sortParallel(less);

    // Work on task, if not already done
    recurseTask.workForce;
}
