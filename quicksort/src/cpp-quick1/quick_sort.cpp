#include <algorithm>
#include <iostream>
#include <memory>
#include <cstdlib>

#include <ttracker.h>

#include "sort/sort.hpp"
#include "sort/sort_utils.hpp"
#include "file/file_utils.h"

/* Defines for time tracking */
#define TTRACKER_MAIN    0 ///< Main function
#define TTRACKER_PARSE   1 ///< Parsing & file reading
#define TTRACKER_SORT    2 ///< Sorting the array
#define TTRACKER_VERIFY  3 ///< Verifying the array
#define TTRACKER_TOTAL   4 ///< Total events tracked

/**
 * Reads a number list from argv and sorts the list using quick sort
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
        std::cout << "Usage: " <<  argv[0] << " array_file [thread_count=1]\n";
        return EXIT_FAILURE;
    }

    int thread_count = 1; // Initialize with default thread count

    if (argc == 3)
    {
        thread_count = std::atoi(argv[2]);

        if (thread_count <= 0)
        {
            std::cout << "Invalid thread_count. Use at least 1!\n";
            return EXIT_FAILURE;
        }
    }

    ttracker_start(&ttracker, TTRACKER_PARSE);
    auto array_string = std::shared_ptr<char>(read_file(argv[1]), free);

    if (array_string == NULL)
    {
        std::cout << "Could not read array_file!\n";
        return EXIT_FAILURE;
    }

    std::vector<unsigned long> vector;
    unsigned long long vector_size;

    try
    {
        vector_size = sort_check_and_parse_length(array_string);
        vector = sort_parse_numbers(vector_size, array_string);
    }
    catch (const std::exception& ex)
    {
        std::cout << ex.what() << "\n";
        return EXIT_FAILURE;
    }
    ttracker_stop(&ttracker,TTRACKER_PARSE);

    ttracker_start(&ttracker, TTRACKER_SORT);
    sort(vector, thread_count);
    ttracker_stop(&ttracker, TTRACKER_SORT);

    ttracker_start(&ttracker, TTRACKER_VERIFY);
    if (!std::is_sorted(vector.begin(), vector.end()))
    {
        std::cout << "Could not sort array!\n";
        return EXIT_FAILURE;
    }
    ttracker_stop(&ttracker, TTRACKER_VERIFY);

    ttracker_stop(&ttracker, TTRACKER_MAIN);
    ttracker_print_sec(&ttracker);

    return EXIT_SUCCESS;
}
