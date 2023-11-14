#ifndef SORT_HPP
#define SORT_HPP

#include <vector>

#include "../taskpool/taskpool.hpp"
#include "../taskpool/qs_task.hpp"

/**
 * Exception for parsing errors
 */
class sort_parser_exception : public std::exception
{
public:
    const char* what() const noexcept override;
};

/**
 * Sorts the vector using Quicksort
 *
 * @param vector The vector to be sorted
 * @param thread_count Thread count
 */
void sort(std::vector<unsigned long>& vector, const unsigned int thread_count);

/**
 * Sorts a range between to iterators using parallel Quicksort
 *
 * @param taskpool The Taskpool to insert new tasks
 * @param task Current task
 * @param tid Thread ID
 */
void sort_parallel(taskpool* taskpool, qs_task* task, unsigned int tid);

#endif
