/*
 * This Code is based on the UCL Research IT Services repository
 * https://github.com/UCL-RITS/pi_examples
 */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include <ttracker.h>

/* Defines for time tracking */
#define TTRACKER_MAIN    0 ///< Main function
#define TTRACKER_CALC    1 ///< Pi calculation
#define TTRACKER_TOTAL   2 ///< Total events tracked

/**
 * This is used as argument for the worker threads
 */
typedef struct _pi_args_t
{
    long step_count;    ///< Step count
    int thread_count;   ///< Thread count
    int tid;            ///< Thread index
    double value;       ///< Calculated value
} pi_args_t;

/**
 * Calculates a part of pi
 * 
 * @param pthread_args Args of the function 
 * @return NULL
 */
void* worker_thread(void *pthread_args)
{
    pi_args_t* args = (pi_args_t*) pthread_args;

    double sum = 0.0;
    double step = 1.0 / args->step_count;
    long lower = args->tid * (args->step_count / args->thread_count);
    long upper = (args->tid + 1) * (args->step_count / args->thread_count);

    if (args->tid == (args->thread_count - 1))
    {
        upper = args->step_count;
    }

    for (long i = lower; i < upper; ++i)
    {
        double x = (i + 0.5) * step;
        sum += 4.0 / (1.0 + x * x);
    }
    
    args->value = sum;
    return NULL;
}

/**
 * Reads the number of steps and calculates pi
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
        printf("Usage: %s step_count [thread_count=1]\n", argv[0]);
        return EXIT_FAILURE;
    }

    int step_count;
    int thread_count = 1; // Initialize with default thread count

    step_count = atoi(argv[1]);

    if (step_count <= 0)
    {
        printf("Invalid step_count. Use at least 1\n");
        return EXIT_FAILURE;
    }
    
    if (argc == 3)
    {
        thread_count = atoi(argv[2]);

        if (thread_count <= 0)
        {
            printf("Invalid thread_count. Use at least 1!\n");
            return EXIT_FAILURE;
        }
    }

    ttracker_start(&ttracker, TTRACKER_CALC);

    int spawned_count = thread_count - 1; // Main thread also works
    pi_args_t args[thread_count];
    pthread_t threads[spawned_count]; 
    
    /* Create thread args */
    for (int i = 0; i < thread_count; ++i)
    {
        args[i].step_count = step_count;
        args[i].thread_count = thread_count;
        args[i].tid = i;
    }

    /* Create threads */
    for (int i = 0; i < spawned_count; ++i)
    {
        pthread_create(&threads[i], NULL, worker_thread, (void*) &args[i + 1]);
    }

    worker_thread((void*) &args[0]);

    /* Wait for all threads to finish */
    for (int i = 0; i < spawned_count; ++i)
    {
        pthread_join(threads[i], NULL);
    }

    double sum = 0.0;

    for (int i = 0; i < thread_count; ++i)
    {
        sum += args[i].value;
    }

    double step = 1.0 / step_count;
    double pi = sum * step;

    ttracker_stop(&ttracker, TTRACKER_CALC);

    /* Prevent the compiler from optimizing away the pi variable */
    FILE* fp = fopen("/dev/null", "w");
    fprintf(fp, "%f", pi);
    fclose(fp);

    ttracker_stop(&ttracker, TTRACKER_MAIN);

    ttracker_print_sec(&ttracker);

    return EXIT_SUCCESS;
}
