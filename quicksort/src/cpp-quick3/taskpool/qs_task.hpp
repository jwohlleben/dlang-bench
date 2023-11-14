#ifndef QS_TASK_HPP
#define QS_TASK_HPP

#include <mutex>
#include <vector>
#include <barrier>
#include <condition_variable>

/**
 * This class represents quicksort task
 */
struct qs_task
{
    qs_task(std::vector<unsigned long>& vector_to_sort,
        unsigned long long first_index, unsigned long long last_index,
        std::vector<unsigned long>& temp, unsigned int necessary_threads);
    /**
     * How many threads are needed for computation
     */
    const unsigned int necessary_threads;

    /**
     * The pivot element
     */
    unsigned long pivot;

    /**
     * How many threads are currently available
     */
    unsigned int assigned_threads;

    /**
     * First element to sort
     */
    unsigned long long first_index;

    /**
     * Last element to sort
     */
    unsigned long long last_index;

    /**
     * Vector to sort
     */
    std::vector<unsigned long>& vector_to_sort;

    /**
     * Auxiliary vector
     */
    std::vector<unsigned long>& temp;

    /**
     * Vector for sharing smaller partition sizes
     */
    std::vector<unsigned long> smaller_partition_size;

    /**
     * Vector for sharing larger partition sizes
     */
    std::vector<unsigned long> larger_partition_size;

    /**
     * Vector for sharing pivot elements
     */
    std::vector<unsigned long> pivot_elements;

    /**
     * Mutex for adding new threads
     */
    std::mutex mutex;

    /**
     * Conditonal variable for the mutex
     */
    std::condition_variable condition;

    /**
     * Barrier for synchronisation
     */
    std::barrier<> barrier;

    /**
     * Task is ready
     */ 
    bool ready() noexcept;

};

#endif
