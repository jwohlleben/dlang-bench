#include "sort.hpp"

#include <algorithm>
#include <iterator>

#include "../taskpool/taskpool.hpp"

const char* sort_parser_exception::what() const noexcept
{
    return "Could not parse numbers array!";
}

void sort(std::vector<unsigned long>& vector, const unsigned int thread_count)
{
    if (thread_count <= 1)
    {
        std::sort(vector.begin(), vector.end());
    }
    else
    {
        taskpool tasks(thread_count - 1);
        tasks.start();
        sort_parallel(vector.begin(), vector.end(), tasks);
        tasks.work_until_finished();
    }
}

void sort_parallel(std::vector<unsigned long>::iterator first,
    std::vector<unsigned long>::iterator last, taskpool& tasks)
{
    auto distance = std::distance(first, last);

    if (distance <= 1)
    {
        return;
    }

    auto pivot = *std::next(first, distance / 2);

    auto middle1 = std::partition(first, last, [pivot](const auto& em)
    {
        return em < pivot;
    });

    auto middle2 = std::partition(middle1, last, [pivot](const auto& em)
    {
        return !(pivot < em);
    });

    // Put right side in taskpool
    tasks.put([middle2, last, &tasks]() {
        sort_parallel(middle2, last, tasks);
    });

    // Work on left side
    sort_parallel(first, middle1, tasks);
}
