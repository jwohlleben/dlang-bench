#ifndef SORT_HPP
#define SORT_HPP

#include <vector>

#include "../taskpool/taskpool.hpp"

/**
 * Exception for parsing errors
 */
class sort_parser_exception : public std::exception
{
public:
    const char* what() const noexcept override;
};

/**
 * Sorts the vector using
 * Introsort, if threadCount <= 1
 * Quicksort, if threadCount >  1
 *
 * @param vector The vector to be sorted
 * @param thread_count Thread count
 */
void sort(std::vector<unsigned long>& vector, const unsigned int thread_count);

/**
 * Sorts a range between to iterators using parallel quicksort
 *
 * This function is inspired from the example at
 * https://en.cppreference.com/w/cpp/algorithm/partition
 *
 * @param first First iterator
 * @param last Last iterator
 * @param tasks Taskpool
 */
void sort_parallel(std::vector<unsigned long>::iterator first,
    std::vector<unsigned long>::iterator last, taskpool& tasks);

#endif
