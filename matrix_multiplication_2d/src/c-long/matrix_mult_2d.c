#include <stdio.h>
#include <stdlib.h>

#include <ttracker.h>

#include "matrix/matrix.h"
#include "file/file_utils.h"
#include "matrix/matrix_utils.h"

/* Defines for time tracking */
#define TTRACKER_MAIN    0 ///< Main function
#define TTRACKER_PARSING 1 ///< Parsing & file reading
#define TTRACKER_MULT    2 ///< Matrix multiplication
#define TTRACKER_WRITE   3 ///< Writing matrix to file
#define TTRACKER_TOTAL   4 ///< Total events tracked

/**
 * Reads two matrix files and the thread_count from argv and outputs
 * the result of the matrix multiplication
 *
 * @param argc Argument count
 * @param argv Argument strings
 * @return EXIT_SUCCESS, if successful
 */
int main(int argc, char* argv[])
{
    ttracker_t ttracker;
    ttracker_event_t ttracker_events[TTRACKER_TOTAL];
    ttracker_init(&ttracker, ttracker_events, TTRACKER_TOTAL);
    ttracker_start(&ttracker, TTRACKER_MAIN);

    if (argc < 4 || argc > 5)
    {
        printf("Usage: %s file1 file2 outfile [thread_count=1]\n", argv[0]);
        return EXIT_FAILURE;
    }

    int thread_count = 1; // Initialize with default thread count

    if (argc == 5)
    {
        thread_count = atoi(argv[4]);

        if (thread_count <= 0)
        {
            printf("Invalid thread_count. Use at least 1!\n");
            return EXIT_FAILURE;
        }
    }

    matrix_t matrix1;
    matrix_t matrix2;
    matrix_t result;

    ttracker_start(&ttracker, TTRACKER_PARSING);

    char* matrix_as_string = read_file(argv[1]);
    if (matrix_as_string == NULL)
    {
        printf("Could not read matrix1 file!\n");
        return EXIT_FAILURE;
    }

    int error_occurred = 0;

    error_occurred = matrix_from_string(matrix_as_string, &matrix1);
    if (error_occurred)
    {
        printf("Could not parse matrix 1!\n");
        free(matrix_as_string);
        return EXIT_FAILURE;
    }

    free(matrix_as_string);
    matrix_as_string = read_file(argv[2]);

    if (matrix_as_string == NULL)
    {
        printf("Could not read matrix2 file!\n");
        matrix_cleanup(&matrix1);
        return EXIT_FAILURE;
    }

    error_occurred = matrix_from_string(matrix_as_string, &matrix2);
    if (error_occurred)
    {
        printf("Could not parse matrix 2!\n");
        free(matrix_as_string);
        matrix_cleanup(&matrix1);
        return EXIT_FAILURE;
    }

    free(matrix_as_string);

    ttracker_stop(&ttracker, TTRACKER_PARSING);

    ttracker_start(&ttracker, TTRACKER_MULT);

    if (thread_count == 1)
    {
        error_occurred = matrix_mult(&matrix1, &matrix2, &result);
    }
    else
    {
        error_occurred =
            matrix_mult_parallel(&matrix1, &matrix2, &result, thread_count);
    }

    ttracker_stop(&ttracker, TTRACKER_MULT);

    if (error_occurred)
    {
        printf("Could not perfrom multiplication!\n");
        matrix_cleanup(&matrix1);
        matrix_cleanup(&matrix2);
        return EXIT_FAILURE;
    }

    ttracker_start(&ttracker, TTRACKER_WRITE);

    if (matrix_write_file(&result, argv[3]))
    {
        printf("Could not write result matrix to file!\n");
        matrix_cleanup(&matrix1);
        matrix_cleanup(&matrix2);
        matrix_cleanup(&result);
        return EXIT_FAILURE;
    }

    ttracker_stop(&ttracker, TTRACKER_WRITE);

    matrix_cleanup(&matrix1);
    matrix_cleanup(&matrix2);
    matrix_cleanup(&result);

    ttracker_stop(&ttracker, TTRACKER_MAIN);

    if (ttracker.status != TTRACKER_SUCCESS)
    {
        printf("Error with measuring time!\n");
        return EXIT_FAILURE;
    }

    ttracker_print_sec(&ttracker);

    return EXIT_SUCCESS;
}
