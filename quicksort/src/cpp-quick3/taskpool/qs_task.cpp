#include "qs_task.hpp"

#include <vector>

qs_task::qs_task(std::vector<unsigned long>& vector_to_sort,
    unsigned long long first_index, unsigned long long last_index,
    std::vector<unsigned long>& temp, unsigned int necessary_threads)
    : necessary_threads(necessary_threads), assigned_threads(0),
        first_index(first_index), last_index(last_index), vector_to_sort(vector_to_sort),
        temp(temp), barrier(necessary_threads)
{
    smaller_partition_size.resize(necessary_threads);
    larger_partition_size.resize(necessary_threads);
    pivot_elements.resize(necessary_threads);
}

bool qs_task::ready() noexcept
{
    return assigned_threads == necessary_threads;
}
