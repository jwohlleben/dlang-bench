#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "matrix.h"
#include "matrix_utils.h"

void* matrix_mult_worker_thread(void* pthread_args)
{
    matrix_args_t* args = (matrix_args_t*) pthread_args;

    /* Thanks to calloc we can directly perform the multiplication */
    for (int index = args->start; index < args->end; ++index)
    {
        int row = matrix_2d_index_row(index, args->result);
        int col = matrix_2d_index_col(index, args->result);

        for (int k = 0; k < args->matrix2->rows; ++k)
        {
            args->result->array[matrix_1d_index(row, col, args->result)] +=
                args->matrix1->array[matrix_1d_index(row, k, args->matrix1)]
                * args->matrix2->array[matrix_1d_index(k, col, args->matrix2)];
        }
    }

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

            break;

        case '+': case '-':
            if (!(expected_token & TOKEN_SIGN))
            {
                return MATRIX_PAR_ERROR;
            }

            expected_token = TOKEN_NUMBER;
            break;

        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
            if (!(expected_token & TOKEN_NUMBER))
            {
                return MATRIX_PAR_ERROR;
            }

            expected_token = TOKEN_NUMBER | TOKEN_COMMA | TOKEN_BRACKET_CLOSE;
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
    matrix->array = (long long int*) malloc(sizeof(long long int) *
        matrix->rows * matrix->cols);

    if (matrix->array == NULL)
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
        case '+': case '-':
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
            matrix->array[matrix_index] = atoll(num_buffer);

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
