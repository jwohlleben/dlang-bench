/*
 * This Code is based on the UCL Research IT Services repository
 * https://github.com/UCL-RITS/pi_examples
 */
import core.thread;

import std.conv;
import std.file;  
import std.stdio;
import std.parallelism;

import cttracker;

version(NO_GC)
{
    import core.memory;
}

/* Defines for time tracking */
enum TTRACKER_MAIN  = 0; ///< Main function
enum TTRACKER_CALC  = 1; ///< Pi calculation
enum TTRACKER_TOTAL = 2; ///< Total events tracked

/* Exit codes */
enum EXIT_SUCCESS = 0; ///< Success
enum EXIT_FAILURE = 1; ///< Failure

/**
 * This is used as argument for the worker threads
 */
struct PiArgs
{
    long stepCount;  ///< Step count
    int threadCount; ///< Thread count
    int tid;         ///< Thread index
    double value;    ///< Calculated value
    int check;
}

/**
 * Calculates a part of pi
 * 
 * @param pthread_args Args of the function 
 */
void workerThread(PiArgs* args)
{
    double sum = 0.0;
    double step = 1.0 / args.stepCount;
    long lower = args.tid * (args.stepCount / args.threadCount);
    long upper = (args.tid + 1) * (args.stepCount / args.threadCount);

    if (args.tid == (args.threadCount - 1))
    {
        upper = args.stepCount;
    }

    for (long i = lower; i < upper; ++i)
    {
        double x = (i + 0.5) * step;
        sum += 4.0 / (1.0 + x * x);
    }

    args.value = sum;
}

/**
 * Reads the number of steps and calculates pi
 *
 * @param argv Argument strings
 * @return EXIT_SUCCESS, if successful
 */
int main(string[] argv)
{
    /* Disable garbage collector, if required */
    version(NO_GC)
    {
        GC.disable();
    }

    ttracker_t ttracker;
    ttracker_event_t[TTRACKER_TOTAL] ttracker_events;
    ttracker_init(&ttracker, ttracker_events.ptr, TTRACKER_TOTAL);
    ttracker_start(&ttracker, TTRACKER_MAIN);

    if (argv.length < 2 || argv.length > 3)
    {
        writeln("Usage: ", argv[0], " step_count [thread_count=1]");
        return EXIT_FAILURE;
    }

    int stepCount;
    int threadCount = 1; // Initialize with default thread count

    try
    {
        stepCount = to!int(argv[1]);
    }
    catch (ConvException e)
    {
        writeln("Invalid step_count. Use at least 1!");
        return EXIT_FAILURE;
    }

    if (stepCount <= 0)
    {
        writeln("Invalid step_count. Use at least 1!");
        return EXIT_FAILURE;
    }

    if (argv.length == 3)
    {
        try
        {
            threadCount = to!int(argv[2]);
        }
        catch (ConvException e)
        {
            writeln("Invalid thread_count. Use at least 1!");
            return EXIT_FAILURE;
        }

        if (threadCount <= 0)
        {
            writeln("Invalid thread_count. Use at least 1!");
            return EXIT_FAILURE;
        }
    }

    ttracker_start(&ttracker, TTRACKER_CALC);

    int spawnedCount = threadCount - 1; // Main thread also works
    PiArgs[] args = new PiArgs[threadCount];

    /* Create thread args */
    for (int i = 0; i < threadCount; ++i)
    {
        args[i].stepCount = stepCount;
        args[i].threadCount = threadCount;
        args[i].tid = i;
    }

    /* Create threads */
    for (int i = 0; i < spawnedCount; ++i)
    {
        task!workerThread(&args[i + 1]).executeInNewThread();
    }

    workerThread(&args[0]);

    thread_joinAll(); // Wait for other threads to finish

    double sum = 0;

    for (int i = 0; i < threadCount; ++i)
    {
        sum += args[i].value;
    }

    double step = 1.0 / stepCount;
    double pi = sum * step;

    ttracker_stop(&ttracker, TTRACKER_CALC);

    /* Prevent the compiler from optimizing away the pi variable */
    std.file.write("/dev/null", to!string(pi));

    ttracker_stop(&ttracker, TTRACKER_MAIN);

    ttracker_print_sec(&ttracker);

    return EXIT_SUCCESS;
}
