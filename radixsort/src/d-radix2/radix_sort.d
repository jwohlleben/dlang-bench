import std.algorithm;
import std.conv;
import std.file;
import std.stdio;

import my_sort;
import cttracker;

version(NO_GC)
{
    import core.memory;
}

/* Defines for time tracking */
enum TTRACKER_MAIN   = 0; ///< Main function
enum TTRACKER_PARSE  = 1; ///< Parsing & file reading
enum TTRACKER_SORT   = 2; ///< Sorting the numbers
enum TTRACKER_VERIFY = 3; ///< Verifying the order
enum TTRACKER_TOTAL  = 4; ///< Total events tracked

/* Exit codes */
enum EXIT_SUCCESS = 0; ///< Success
enum EXIT_FAILURE = 1; ///< Failure

/**
 * Reads a number list from argv and sorts the list using radix sort
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
        writeln("Usage: ", argv[0], " array_file [thread_count=1]");
        return EXIT_FAILURE;
    }

    int threadCount = 1; // Initialize with default thread count

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

    SortMemory memory;

    try
    {
        ttracker_start(&ttracker, TTRACKER_PARSE);
        string arrayString = readText(argv[1]);
        sortInitMemory(arrayString, memory, threadCount);
        ttracker_stop(&ttracker, TTRACKER_PARSE);
    }
    catch (FileException e)
    {
        writeln("Error with opening array_file!");
        return EXIT_FAILURE;
    }
    catch (SortParserException e)
    {
        writeln("Could not parse array_file!");
        return EXIT_FAILURE;
    }

    ttracker_start(&ttracker, TTRACKER_SORT);
    mySort(memory);
    ttracker_stop(&ttracker, TTRACKER_SORT);

    ttracker_start(&ttracker, TTRACKER_VERIFY);
    if (!isSorted(memory.array))
    {
        writeln("Could not sort array!");
        return EXIT_FAILURE;
    }
    ttracker_stop(&ttracker, TTRACKER_VERIFY);

    ttracker_stop(&ttracker, TTRACKER_MAIN);
    ttracker_print_sec(&ttracker);

    return EXIT_SUCCESS;
}
