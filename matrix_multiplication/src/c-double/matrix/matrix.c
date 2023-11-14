#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "matrix.h"
#include "matrix_utils.h"

int matrix_from_string(const char* matrix_as_string, matrix_t* matrix)
{
    if (matrix_check_and_parse_dimensions(matrix_as_string, matrix))
    {
        return MATRIX_PAR_ERROR;
    }

    if (matrix_parse_numbers(matrix_as_string, matrix))
    {
        return MATRIX_MEM_ERROR;
    }

    return MATRIX_SUCCESS;
}

int matrix_mult(const matrix_t* matrix1, const matrix_t* matrix2,
    matrix_t* result)
{
    if (matrix1->cols != matrix2->rows)
    {
        return MATRIX_DIM_ERROR;
    }

    result->rows = matrix1->rows;
    result->cols = matrix2->cols;

    /* Allocate and check memory using calloc */
    result->array = (double*) calloc(result->rows * result->cols,
        sizeof(double));

    if (result->array == NULL)
    {
        return MATRIX_MEM_ERROR;
    }

    /* Thanks to calloc we can directly perform the multiplication */
    for (int i = 0; i < result->rows; ++i)
    {
        for (int j = 0; j < result->cols; ++j)
        {
            for (int k = 0; k < matrix2->rows; ++k)
            {
                result->array[matrix_1d_index(i, j, result)] +=
                    matrix1->array[matrix_1d_index(i, k, matrix1)]
                    * matrix2->array[matrix_1d_index(k, j, matrix2)];
            }
        }
    }

    return MATRIX_SUCCESS;
}

int matrix_mult_parallel(const matrix_t* matrix1, const matrix_t* matrix2,
    matrix_t* result, unsigned int thread_count)
{
    if (matrix1->cols != matrix2->rows)
    {
        return MATRIX_DIM_ERROR;
    }

    result->rows = matrix1->rows;
    result->cols = matrix2->cols;

    /* Allocate and check memory using calloc */
    int array_length = result->rows * result->cols;
    result->array = (double*) calloc(array_length,
        sizeof(double));

    if (result->array == NULL)
    {
        return MATRIX_MEM_ERROR;
    }

    /* Create threads & args for every thread */
    unsigned int spawned_count = thread_count - 1; // Excludes main thread
    pthread_t threads[thread_count - 1];
    matrix_args_t args[thread_count];

    unsigned int indizes_per_thread =
        (unsigned int) array_length / thread_count;

    unsigned int remaining_indizes = array_length % thread_count;
    unsigned int index_offset = 0;

    /* Distribute target indices fairly across threads*/
    for (int i = 0; i < thread_count; ++i)
    {
        args[i].matrix1 = matrix1;
        args[i].matrix2 = matrix2;
        args[i].result = result;

        args[i].start = i * indizes_per_thread + index_offset;
        args[i].end = (i + 1) * indizes_per_thread + index_offset;

        if (remaining_indizes != 0)
        {
            ++args[i].end;
            ++index_offset;
            --remaining_indizes;
        }
    }

    /* Spawn worker threads */
    for (int i = 0; i < spawned_count; ++i)
    {
        pthread_create(&threads[i], NULL,
            matrix_mult_worker_thread, (void*) &args[i + 1]);
    }

    // Main thread also calculates
    matrix_mult_worker_thread(&args[0]);

    /* Wait for all threads to finish */
    for (int i = 0; i < spawned_count; ++i)
    {
        pthread_join(threads[i], NULL);
    }

    return MATRIX_SUCCESS;
}

int matrix_write_file(const matrix_t* matrix, const char* filename)
{
    FILE* fp = fopen(filename, "w");
    char buffer[MATRIX_BUFF_SIZE] = {0};

    if (fp == NULL)
    {
        return MATRIX_FILE_ERROR;
    }

    fputc('[', fp);

    for (int row = 0; row < matrix->rows; ++row)
    {
        fputc('[', fp);

        for (int col = 0; col < matrix->cols; ++col)
        {
            sprintf(buffer, "%lf",
                matrix->array[matrix_1d_index(row, col, matrix)]);

            fputs(buffer, fp);
            memset(buffer, 0, MATRIX_BUFF_SIZE);

            if (col != matrix->cols - 1)
            {
                fputc(',', fp);
            }
        }

        fputc(']', fp);

        if (row != matrix->rows - 1)
        {
            fputc(',', fp);
        }
    }

    fputs("]\n", fp);

    if (ferror(fp))
    {
        fclose(fp);
        return MATRIX_FILE_ERROR;
    }

    fclose(fp);

    return MATRIX_SUCCESS;
}
