module my_sort;

import std.conv;
import std.parallelism;
import core.thread;
import core.sync.barrier;

import sort_utils;

/**
 * Represents the complete radix sort memory
 */
struct SortMemory
{
    uint[] array;       ///< Array to be sorted
    uint[] temp;        ///< Temporary swapping array
    ulong[] zeroCount;  ///< Zero count array
    ulong[] oneCount;   ///< One count array
    uint threadCount;   ///< Number of threads
    ubyte maxBits;      ///< Bit count of the biggest number
}

/**
 * Arguments for a worker thread
 */
struct SortArgs
{
    ulong startIndex;   ///< Start index for sorting
    ulong endIndex;     ///< End index for sorting
    uint threadIndex;   ///< Index of the thread
    SortMemory memory;  ///< Memory of radix sort
    Barrier barrier;    ///< Barrier for synchronization
}

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
 * Initializes the radix sort memory
 *
 * @param arrayString Array as string
 * @param memory Memory to be initialized
 * @param threadCount Threads to use for sorting
 */
void sortInitMemory(const ref string arrayString, ref SortMemory memory,
    uint threadCount)
{
    ulong length = sortCheckAndParseLength(arrayString);
    
    memory.threadCount = threadCount;

    memory.array = new uint[length];
    memory.temp = new uint[length];
    memory.zeroCount = new ulong[threadCount];
    memory.oneCount = new ulong[threadCount];

    sortParseNumbers(arrayString, memory);
}

/**
 * Sorts the array in the memory using radix sort
 *
 * @param memory Memory for sorting
 */
void mySort(ref SortMemory memory)
{
    SortArgs[] args = new SortArgs[memory.threadCount];
    Barrier barrier = new Barrier(memory.threadCount);
    
    uint spawnedCount = memory.threadCount - 1; // -main thread
    ulong indizesPerThread = to!ulong(memory.array.length / memory.threadCount);
    uint remainingIndizes = memory.array.length % memory.threadCount;
    uint indexOffset = 0;

    /* Distribute target indices fairly across threads*/
    for (int i = 0; i < memory.threadCount; ++i)
    {
        args[i].startIndex = i * indizesPerThread + indexOffset;
        args[i].endIndex = (i + 1) * indizesPerThread + indexOffset;
        args[i].threadIndex = i;
        args[i].memory = memory;
        args[i].barrier = barrier;

        if (remainingIndizes != 0)
        {
            ++args[i].endIndex;
            ++indexOffset;
            --remainingIndizes;
        }
    }

    /* Spawn worker threads */
    for (int i = 0; i < spawnedCount; ++i)
    {
       auto task = task!sortWorkerThread(args[i + 1]);
       task.executeInNewThread();
    }

    // Main thread also works
    sortWorkerThread(args[0]);

    // Wait for all threads to finish
    thread_joinAll();

    /* Swap array again, if result array is currently in temp */
    if (memory.maxBits % 2 == 1)
    {
        uint[] result = memory.temp;
        memory.temp = memory.array;
        memory.array = result;
    }
}

/**
 * Represents a worker unit for sorting
 *
 * @param args Sorting arguments
 */
void sortWorkerThread(ref SortArgs args)
{
    SortMemory memory = args.memory;

    ubyte bit;
    uint[] temp;
    uint[] srcArray = memory.array;
    uint[] destArray = memory.temp;
    ulong i;
    ulong zeroIndex;
    ulong oneIndex;

    /* Iterate through each bit */
    for (bit = 0; bit < memory.maxBits; ++bit)
    {
        memory.zeroCount[args.threadIndex] = 0;
        memory.oneCount[args.threadIndex] = 0;

        /* Count zeroes and ones */
        for (i = args.startIndex; i < args.endIndex; ++i)
        {
            if (((srcArray[i] >> bit) & 1) == 0)
            {
                ++memory.zeroCount[args.threadIndex];
            }
            else
            {
                ++memory.oneCount[args.threadIndex];
            }
        }

        args.barrier.wait();

        zeroIndex = 0;
        oneIndex = 0;

        /* Calculate index offset */
        for (i = 0; i < args.threadIndex; ++i)
        {
            zeroIndex += memory.zeroCount[i];
            oneIndex += memory.oneCount[i];
        }

        oneIndex += zeroIndex;

        for (; i < memory.threadCount; ++i)
        {
            oneIndex += memory.zeroCount[i];
        }

        args.barrier.wait();

        /* Write back new order */
        for (i = args.startIndex; i < args.endIndex; ++i)
        {
            if (((srcArray[i] >> bit) & 1) == 0)
            {
                destArray[zeroIndex] = srcArray[i];
                ++zeroIndex;
            }
            else
            {
                destArray[oneIndex] = srcArray[i];
                ++oneIndex;
            }
        }

        args.barrier.wait();

        /* Swap arrays */
        temp = srcArray;
        srcArray = destArray;
        destArray = temp;
    }
}
