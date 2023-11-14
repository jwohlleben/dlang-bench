#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "matrix.h"
#include "matrix_utils.h"

void matrix_mult_worker_thread_range(matrix_args_t* args, int start_row, 
    int end_row, int start_col, int end_col)
{
    for (int i = start_row; i <= end_row; ++i)
    {
        for (int j = start_col; j <= end_col; ++j)
        {
            for (int k = 0; k < args->matrix2->rows; ++k)
            {
                args->result->array[i][j] += args->matrix1->array[i][k]
                    * args->matrix2->array[k][j];
            }
        }
    }
}

void* matrix_mult_worker_thread(void* pthread_args)
{
    matrix_args_t* args = (matrix_args_t*) pthread_args;

    /* Finish first row */
    if (args->start_row == args->end_row)
    {
        matrix_mult_worker_thread_range(args, args->start_row,
            args->end_row, args->start_col, args->end_col);

        return NULL;
    }
    else
    {
        matrix_mult_worker_thread_range(args, args->start_row,
            args->start_row, args->start_col, (int) args->result->cols - 1);
    }

    /* Calculate all mid rows */
    matrix_mult_worker_thread_range(args, args->start_row + 1,
        (int) args->end_row - 1, 0, (int) args->result->cols - 1);

    /* Finish last row */
    matrix_mult_worker_thread_range(args, args->end_row, args->end_row, 0,
        args->end_col);

    return NULL;
}

int matrix_check_and_parse_dimensions(const char* matrix_as_string,
    matrix_t* matrix)
{
    unsigned int rows = 0;
    unsigned int cols = 0;
    int inner_commas = 0;
    int outer_commas = 0;
    int bracket_count = 0;
    bool seen_colon = false;
    unsigned char expected_token = TOKEN_BRACKET_OPEN;
    char token;

    /* Parse string and set dimensions */
    while ((token = *(matrix_as_string++)))
    {
        switch (token)
        {
        case '[':
            if (!(expected_token & TOKEN_BRACKET_OPEN))
            {
                return MATRIX_PAR_ERROR;
            }

            if (bracket_count < 1)
            {
                expected_token = TOKEN_BRACKET_OPEN;
            }
            else
            {
                expected_token = TOKEN_SIGN | TOKEN_NUMBER;
            }

            ++bracket_count;
            break;

        case ']':
            if (!(expected_token & TOKEN_BRACKET_CLOSE))
            {
                return MATRIX_PAR_ERROR;
            }
            if (bracket_count == 2)
            {
                if (cols == 0)
                {
                    cols = inner_commas + 1;
                }
                else
                {
                    if (cols != (inner_commas + 1))
                    {
                        return MATRIX_PAR_ERROR;
                    }
                }

                inner_commas = 0;
                expected_token = TOKEN_BRACKET_CLOSE | TOKEN_COMMA;
            }
            else if (bracket_count == 1)
            {
                rows = outer_commas + 1;
                expected_token = TOKEN_NONE;
            }

            --bracket_count;
            break;

        case ',':
            if (!(expected_token & TOKEN_COMMA))
            {
                return MATRIX_PAR_ERROR;
            }

            switch (bracket_count)
            {
            case 1:
                ++outer_commas;
                expected_token = TOKEN_BRACKET_OPEN;
                break;

            case 2:
                ++inner_commas;
                expected_token = TOKEN_SIGN | TOKEN_NUMBER;
                break;
            }

            seen_colon = false;
            break;

        case '+': case '-':
            if (!(expected_token & TOKEN_SIGN))
            {
                return MATRIX_PAR_ERROR;
            }

            expected_token = TOKEN_NUMBER;
            break;

        case '.':
            if (!(expected_token & TOKEN_COLON))
            {
                return MATRIX_PAR_ERROR;
            }

            expected_token = TOKEN_NUMBER;
            seen_colon = true;
            break;

        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
            if (!(expected_token & TOKEN_NUMBER))
            {
                return MATRIX_PAR_ERROR;
            }

            expected_token = TOKEN_NUMBER | TOKEN_COMMA | TOKEN_BRACKET_CLOSE;

            if (!seen_colon)
            {
                expected_token |= TOKEN_COLON;
            }

            break;

        case ' ': case '\n':
            break;

        default:
            return MATRIX_PAR_ERROR;
        }
    }

    matrix->rows = rows;
    matrix->cols = cols;

    return MATRIX_SUCCESS;
}

int matrix_parse_numbers(const char* matrix_as_string,
    matrix_t* matrix)
{
    /* Allocate and check memory */
    if (matrix_malloc(matrix))
    {
        return MATRIX_MEM_ERROR;
    }

    bool num_available = false;
    int matrix_index = 0;
    int buffer_index = 0;
    char num_buffer[MATRIX_BUFF_SIZE] = {0};
    char token;

    /* Parse numbers */
    while ((token = *(matrix_as_string++)))
    {
        switch (token)
        {
        case '+': case '-': case '.':
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
            num_available = true;

            // Fill buffer with tokens
            num_buffer[buffer_index] = token;
            ++buffer_index;
            break;

        case ',': case ']':
            if (!num_available)
            {
                break;
            }
            num_available = false;
            int row = matrix_2d_index_row(matrix_index, matrix);
            int col = matrix_2d_index_col(matrix_index, matrix);
            matrix->array[row][col] = atoll(num_buffer);

            // Reset buffer
            memset(num_buffer, 0, buffer_index + 1);
            buffer_index = 0;

            ++matrix_index;
            break;

        default:
            break;
        }
    }

    return MATRIX_SUCCESS;
}

int matrix_malloc(matrix_t* matrix)
{
    matrix->array = (long long int**) malloc(sizeof(long long int*)
        * matrix->rows);

    if (matrix->array == NULL)
    {
        return MATRIX_MEM_ERROR;
    }

    for (int row = 0; row < matrix->rows; ++row)
    {
        matrix->array[row] = NULL;
    }

    for (int row = 0; row < matrix->rows; ++row)
    {
        matrix->array[row] = (long long int*) calloc(matrix->cols,
            sizeof(long long int));
        
        if (matrix->array[row] == NULL)
        {
            matrix_cleanup(matrix);
            return MATRIX_MEM_ERROR;
        }
    }

    return MATRIX_SUCCESS;
}

void matrix_cleanup(matrix_t* matrix)
{
    if (matrix->array == NULL) return;

    for (int row = 0; row < matrix->rows; ++row)
    {
        free(matrix->array[row]);
    }

    free(matrix->array);
    matrix->array = NULL;
}
