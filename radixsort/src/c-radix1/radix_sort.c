#include <stdio.h>
#include <stdlib.h>

#include <ttracker.h>

#include "sort/sort.h"
#include "file/file_utils.h"

/* Defines for time tracking */
#define TTRACKER_MAIN    0 ///< Main function
#define TTRACKER_PARSE   1 ///< Parsing & file reading
#define TTRACKER_SORT    2 ///< Sorting the array
#define TTRACKER_VERIFY  3 ///< Verifying the array
#define TTRACKER_TOTAL   4 ///< Total events tracked

/**
 * Reads a number list from argv and sorts the list using radix sort
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

    if (argc < 2 || argc > 3)
    {
        printf("Usage: %s array_file [thread_count=1]\n", argv[0]);
        return EXIT_FAILURE;
    }

    int thread_count = 1; // Initialize with default thread count

    if (argc == 3)
    {
        thread_count = atoi(argv[2]);

        if (thread_count <= 0)
        {
            printf("Invalid thread_count. Use at least 1!\n");
            return EXIT_FAILURE;
        }
    }

    ttracker_start(&ttracker, TTRACKER_PARSE);
    char* array_string = read_file(argv[1]);

    if (array_string == NULL)
    {
        printf("Could not read array_file!\n");
        return EXIT_FAILURE;
    }

    sort_memory_t memory;

    if (sort_init_memory(array_string, &memory, thread_count))
    {
        printf("Could not initialize sort memory!\n");
        free(array_string);
        return EXIT_FAILURE;
    }

    free(array_string);
    ttracker_stop(&ttracker,TTRACKER_PARSE);

    ttracker_start(&ttracker, TTRACKER_SORT);
    sort(&memory);
    ttracker_stop(&ttracker, TTRACKER_SORT);

    ttracker_start(&ttracker, TTRACKER_VERIFY);
    if (sort_verify_sorted(&memory))
    {
        printf("Could not sort array!\n");
        sort_cleanup_memory(&memory);
        return EXIT_FAILURE;
    }
    ttracker_stop(&ttracker, TTRACKER_VERIFY);

    sort_cleanup_memory(&memory);

    ttracker_stop(&ttracker, TTRACKER_MAIN);
    ttracker_print_sec(&ttracker);

    return EXIT_SUCCESS;
}
