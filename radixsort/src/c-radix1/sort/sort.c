#include "sort.h"

#include <stdlib.h>
#include <pthread.h>

#include "sort_utils.h"

int sort_init_memory(const char* array_string, sort_memory_t* memory,
    unsigned int thread_count)
{
    memory->thread_count = thread_count;
    memory->length = 0;
    memory->max_bits = 0;

    if (sort_check_and_parse_length(array_string, memory))
    {
        return SORT_FAILURE;
    }

    memory->array = (unsigned long*) malloc (
        memory->length * sizeof(unsigned long));

    memory->temp = (unsigned long*) malloc (
        memory->length * sizeof(unsigned long));

    memory->zero_count = (unsigned long long*) malloc (
        thread_count * sizeof(unsigned long));

    memory->one_count = (unsigned long long*) malloc (
        thread_count * sizeof(unsigned long));

    if (memory->array == NULL || memory->temp == NULL
        || memory->zero_count == NULL || memory->one_count == NULL)
    {
        sort_cleanup_memory(memory);
        return SORT_FAILURE;
    }

    sort_parse_numbers(array_string, memory);

    return SORT_SUCCESS;
}

void sort_cleanup_memory(sort_memory_t* memory)
{
    free(memory->array);
    free(memory->temp);
    free(memory->zero_count);
    free(memory->one_count);
    
    memory->array = NULL;
    memory->temp = NULL;
    memory->zero_count = NULL;
    memory->one_count = NULL;

    memory->thread_count = 0;
    memory->length = 0;
    memory->max_bits = 0;
}

void sort(sort_memory_t* memory)
{
    /* Create threads & args for every thread */
    unsigned int spawned_count = memory->thread_count - 1; // -main thread
    pthread_t threads[memory->thread_count - 1];
    sort_args_t args[memory->thread_count];
    pthread_barrier_t barrier;

    pthread_barrier_init(&barrier, NULL, memory->thread_count);

    unsigned long long indizes_per_thread =
        (unsigned long long) memory->length / memory->thread_count;

    unsigned int remaining_indizes = memory->length % memory->thread_count;
    unsigned int index_offset = 0;

    /* Distribute target indices fairly across threads*/
    for (int i = 0; i < memory->thread_count; ++i)
    {
        args[i].start_index = i * indizes_per_thread + index_offset;
        args[i].end_index = (i + 1) * indizes_per_thread + index_offset;
        args[i].thread_index = i;
        args[i].memory = memory;
        args[i].barrier = &barrier;

        if (remaining_indizes != 0)
        {
            ++args[i].end_index;
            ++index_offset;
            --remaining_indizes;
        }
    }

    /* Spawn worker threads */
    for (int i = 0; i < spawned_count; ++i)
    {
        pthread_create(&threads[i], NULL, sort_worker_thread,
            (void*) &args[i + 1]);
    }

    // Main thread works also
    sort_worker_thread((void*) &args[0]);

    /* Wait for all threads to finish */
    for (int i = 0; i < spawned_count; ++i)
    {
        pthread_join(threads[i], NULL);
    }

    pthread_barrier_destroy(&barrier);

    /* Swap array again, if result array is currently in temp */
    if (memory->max_bits % 2 == 1)
    {
        unsigned long* result = memory->temp;
        memory->temp = memory->array;
        memory->array = result;
    }
}

void* sort_worker_thread(void* thread_args)
{
    sort_args_t* args = (sort_args_t*) thread_args;
    sort_memory_t* memory = args->memory;

    char bit;
    unsigned long* temp;
    unsigned long* src_array = memory->array;
    unsigned long* dest_array = memory->temp;
    unsigned long long i;
    unsigned long long zero_index;
    unsigned long long one_index;

    /* Iterate through each bit */
    for (bit = 0; bit < memory->max_bits; ++bit)
    {
        memory->zero_count[args->thread_index] = 0;
        memory->one_count[args->thread_index] = 0;

        /* Count zeroes and ones */
        for (i = args->start_index; i < args->end_index; ++i)
        {
            if (((src_array[i] >> bit) & 1) == 0)
            {
                ++memory->zero_count[args->thread_index];
            }
            else
            {
                ++memory->one_count[args->thread_index];
            }
        }

        pthread_barrier_wait(args->barrier);

        zero_index = 0;
        one_index = 0;

        /* Calculate index offset */
        for (i = 0; i < args->thread_index; ++i)
        {
            zero_index += memory->zero_count[i];
            one_index += memory->one_count[i];
        }

        one_index += zero_index;

        for (; i < memory->thread_count; ++i)
        {
            one_index += memory->zero_count[i];
        }

        pthread_barrier_wait(args->barrier);

        /* Write back new order */
        for (i = args->start_index; i < args->end_index; ++i)
        {
            if (((src_array[i] >> bit) & 1) == 0)
            {
                dest_array[zero_index] = src_array[i];
                ++zero_index;
            }
            else
            {
                dest_array[one_index] = src_array[i];
                ++one_index;
            }
        }

        pthread_barrier_wait(args->barrier);

        /* Swap arrays */
        temp = src_array;
        src_array = dest_array;
        dest_array = temp;
    }

    return NULL;
}

int sort_verify_sorted(const sort_memory_t* memory)
{
    if (memory->length <= 1)
    {
        return SORT_SUCCESS;
    }

    for (unsigned long long i = 1; i < memory->length; ++i)
    {
        if (memory->array[i - 1] > memory->array[i])
        {
            return SORT_FAILURE;
        }
    }

    return SORT_SUCCESS;
}
