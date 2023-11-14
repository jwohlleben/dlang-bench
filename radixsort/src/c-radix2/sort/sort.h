#ifndef SORT_H
#define SORT_H

#include <pthread.h>

/* Defines for sort return codes */
#define SORT_SUCCESS 0x0 ///< Success
#define SORT_FAILURE 0x1 ///< Failure

/**
 * Is used for CPU-Cacheline optimization
 */
typedef union _sort_count_t
{
    unsigned long long count;   ///< Count value
    char pad[128];              ///< Padding
} sort_count_t;

/**
 * Represents the complete radix sort memory
 */
typedef struct _sort_memory_t
{
    unsigned long* array;       ///< Array to be sorted
    unsigned long* temp;        ///< Temporary swapping array
    sort_count_t* zero_count;   ///< Zero count array
    sort_count_t* one_count;    ///< One count array
    unsigned int thread_count;  ///< Number of threads
    unsigned long long length;  ///< Length of the arrays
    unsigned char max_bits;     ///< Bit count of the biggest number
} sort_memory_t;

/**
 * Arguments for a worker thread
 */
typedef struct _sort_args_t
{
    unsigned long long start_index; ///< Start index for sorting
    unsigned long long end_index;   ///< End index for sorting
    unsigned int thread_index;      ///< Index of the thread
    sort_memory_t* memory;          ///< Memory of radix sort
    pthread_barrier_t* barrier;     ///< Barrier for synchronization
} sort_args_t;

/**
 * Initializes the radix sort memory
 *
 * @param array_string Array as string
 * @param memory Memory to be initialized
 * @param thread_count Threads to use for sorting
 * @return SORT_SUCCESS, if successful
 */
int sort_init_memory(const char* array_string, sort_memory_t* memory,
    unsigned int thread_count);

/**
 * Cleans up an initialized radix sort memory
 *
 * @param memory Memory to be cleaned
 */
void sort_cleanup_memory(sort_memory_t* memory);

/**
 * Sorts the array in the memory using radix sort
 *
 * @param memory Memory for sorting
 */
void sort(sort_memory_t* memory);

/**
 * Represents a worker unit for sorting
 *
 * @param thread_args Sorting arguments
 * @return NULL
 */
void* sort_worker_thread(void* thread_args);

/**
 * Verifies that the array is sorted
 *
 * @param memory Memory with array to be verified
 * @return SORT_SUCCESS, if successful
 */
int sort_verify_sorted(const sort_memory_t* memory);

#endif
