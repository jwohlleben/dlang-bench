#include "sort.hpp"

#include <algorithm>
#include <iterator>
#include <memory>

#include "../taskpool/taskpool.hpp"
#include "../taskpool/qs_task.hpp"

const char* sort_parser_exception::what() const noexcept
{
    return "Could not parse numbers array!";
}

void sort(std::vector<unsigned long>& vector, const unsigned int thread_count)
{
    std::vector<unsigned long> temp;
    temp.resize(vector.size());
    taskpool tasks(thread_count - 1);
    tasks.start();
    tasks.put_and_work_until_finished(std::make_shared<qs_task>(
        vector, 0, vector.size(), temp, thread_count
    ));
}

void sort_parallel(taskpool* taskpool, qs_task* task, unsigned int tid)
{
    auto element_count = task->last_index - task->first_index;

    if (element_count <= 1)
    {
        return;
    }

    unsigned int elements_per_thread = element_count / task->assigned_threads;
    unsigned int remaining_elements = element_count % task->assigned_threads;
    unsigned long long start_index;
    unsigned long long end_index;

    /* First thread also computes remaining elements */
    if (tid == 0)
    {
        start_index = task->first_index + tid * elements_per_thread;
        
        end_index = task->first_index
            + (tid + 1) * elements_per_thread + remaining_elements;

        task->pivot = 
            task->vector_to_sort[task->first_index + (element_count / 2) - 1];
    }
    else
    {
        start_index = task->first_index
            + tid * elements_per_thread + remaining_elements;

        end_index = task->first_index
            + (tid + 1) * elements_per_thread + remaining_elements;
    }

    task->barrier.arrive_and_wait();

    auto first_it = task->vector_to_sort.begin() + start_index;
    auto last_it = task->vector_to_sort.begin() + end_index;

    /* Move smaller elements to the left */
    auto smaller_it = std::partition(first_it, last_it, [=](const auto& em)
    {
        return em < task->pivot;
    });

    /* Move larger elements to the right */
    auto larger_it = std::partition(smaller_it, last_it, [=](const auto& em)
    {
       return !(task->pivot < em);
    });

    auto smaller_elements_count = std::distance(first_it, smaller_it);
    auto pivot_elements_count = std::distance(smaller_it, larger_it);
    auto larger_elements_count = std::distance(larger_it, last_it);

    task->smaller_partition_size[tid] = smaller_elements_count;
    task->pivot_elements[tid] = pivot_elements_count;
    task->larger_partition_size[tid] = larger_elements_count;
    
    task->barrier.arrive_and_wait();

    /* Calculate offset for smaller elements */
    auto smaller_elements_offset = task->first_index;
    for (unsigned int i = 0; i < tid; ++i)
    {
        smaller_elements_offset += task->smaller_partition_size[i];
    }

    /* Write smaller elements to temp vector */
    for (unsigned int i = 0; i < smaller_elements_count; ++i)
    {
        task->temp[smaller_elements_offset + i] = *(first_it + i);
    }

    /* Calculate offset for pivot elements */
    auto pivot_elements_offset = task->first_index;
    for (unsigned int i = 0; i < task->necessary_threads; ++i)
    {
        pivot_elements_offset += task->smaller_partition_size[i];
    }
    for (unsigned int i = 0; i < tid; ++i)
    {
        pivot_elements_offset += task->pivot_elements[i];
    }

    /* Write pivot elements to temp vector */
    for (auto i = 0; i < pivot_elements_count; ++i)
    {
        task->temp[pivot_elements_offset + i] = *(smaller_it + i);
    }

    /* Calculate offset for larger elements */
    auto larger_elements_offset = task->last_index;
    for (auto i = tid; i < task->necessary_threads; ++i)
    {
        larger_elements_offset -= task->larger_partition_size[i];
    }

    /* Write larger elements to temp vector */
    for (auto i = 0; i < larger_elements_count; ++i)
    {
        task->temp[larger_elements_offset + i] = *(larger_it + i);
    }

    task->barrier.arrive_and_wait();

    /* Write back elements */
    for (unsigned long i = start_index; i < end_index; ++i)
    {
        task->vector_to_sort[i] = task->temp[i];
    }

    /* Create new task, if first thread */
    if (tid == 0)
    {
        unsigned long smaller_elements_count = 0;
        for (unsigned long i = 0; i < task->necessary_threads; ++i)
        {
            smaller_elements_count += task->smaller_partition_size[i];
        }
        unsigned long smaller_end_index = task->first_index
            + smaller_elements_count;

        unsigned long larger_elements_count = 0;
        for (unsigned long i = 0; i < task->necessary_threads; ++i)
        {
            larger_elements_count += task->larger_partition_size[i];
        }
        unsigned long larger_start_index = task->last_index
            - larger_elements_count;

        auto smaller_necessary_threads = static_cast<unsigned int>(
            smaller_elements_count *
            (taskpool->thread_count() / task->vector_to_sort.size()) + 0.5
        );

        auto larger_necessary_threads = static_cast<unsigned int>(
            larger_elements_count *
            (taskpool->thread_count() / task->vector_to_sort.size()) + 0.5
        );

        smaller_necessary_threads =
            std::max(smaller_necessary_threads, static_cast<unsigned int>(1));
        
        larger_necessary_threads =
            std::max(larger_necessary_threads, static_cast<unsigned int>(1));

        taskpool->put(std::make_shared<qs_task>(
            task->vector_to_sort, task->first_index, smaller_end_index,
            task->temp, smaller_necessary_threads
        ));

        taskpool->put(std::make_shared<qs_task>(
            task->vector_to_sort, larger_start_index, task->last_index,
            task->temp, larger_necessary_threads
        ));
    }
}
