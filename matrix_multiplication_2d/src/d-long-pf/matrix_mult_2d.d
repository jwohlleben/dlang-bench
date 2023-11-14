import std.conv;
import std.file;
import std.stdio;

import matrix;
import cttracker;

version(NO_GC)
{
    import core.memory;
}

/* Defines for time tracking */
enum TTRACKER_MAIN    = 0; ///< Main function
enum TTRACKER_PARSING = 1; ///< Parsing & file reading
enum TTRACKER_MULT    = 2; ///< Matrix multiplication
enum TTRACKER_WRITE   = 3; ///< Writing matrix to file
enum TTRACKER_TOTAL   = 4; ///< Total events tracked

/* Exit codes */
enum EXIT_SUCCESS = 0; ///< Success
enum EXIT_FAILURE = 1; ///< Failure

/**
 * Reads two matrix files and the thread_count from argv and outputs
 * the result of the matrix multiplication
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

    if (argv.length < 4 || argv.length > 5)
    {
        writeln("Usage: ", argv[0], " file1 file2 outfile [thread_count=1]");
        return EXIT_FAILURE;
    }

    int threadCount = 1; // Initialize with default thread count

    if (argv.length == 5)
    {
        try
        {
            threadCount = to!int(argv[4]);
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

    Matrix matrix1;
    Matrix matrix2;
    Matrix result;
    int currentMatrix = 1;

    try
    {
        ttracker_start(&ttracker, TTRACKER_PARSING);

        /* Read and parse first matrix */
        string matrixAsString = readText(argv[1]);
        matrixFromString(matrixAsString, matrix1);

        ++currentMatrix;

        /* Read and parse second matrix */
        matrixAsString = readText(argv[2]);
        matrixFromString(matrixAsString, matrix2);

        ++currentMatrix;

        ttracker_stop(&ttracker, TTRACKER_PARSING);

        ttracker_start(&ttracker, TTRACKER_MULT);

        /* Perform matrix multiplication */
        if (threadCount == 1)
        {
            matrixMult(matrix1, matrix2, result);
        }
        else
        {
            matrixMultParallel(matrix1, matrix2, result, threadCount);
        }

        ttracker_stop(&ttracker, TTRACKER_MULT);

        ttracker_start(&ttracker, TTRACKER_WRITE);
        matrixWriteFile(result, argv[3]);
        ttracker_stop(&ttracker, TTRACKER_WRITE);
    }
    catch (FileException e)
    {
        writeln("Error with opening matrix", currentMatrix, " file!");
        return EXIT_FAILURE;
    }
    catch (MatrixParserException e)
    {
        writeln("Could not parse matrix", currentMatrix, " file!");
        return EXIT_FAILURE;
    }
    catch (MatrixDimensionException e)
    {
        writeln("Could not perfrom multiplication!");
        return EXIT_FAILURE;
    }

    ttracker_stop(&ttracker, TTRACKER_MAIN);

    if (ttracker.status != TTRACKER_SUCCESS)
    {
        writeln("Error with measuring time!\n");
        return EXIT_FAILURE;
    }

    ttracker_print_sec(&ttracker);

    return EXIT_SUCCESS;
}
