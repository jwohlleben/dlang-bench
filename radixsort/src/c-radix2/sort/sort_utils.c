#include "sort_utils.h"

#include <stdlib.h>
#include <string.h>

#include "sort.h"

int sort_check_and_parse_length(const char* array_string, sort_memory_t* memory)
{
    char token;
    char expected_token = TOKEN_NUMBER;
    unsigned long long length = 0;

    while ((token = *(array_string++)))
    {
        switch (token)
        {
        case ',': case '\n':
            if (!(expected_token & TOKEN_BREAK))
            {
                return SORT_FAILURE;
            }

            ++length;
            expected_token = TOKEN_NUMBER;
            break;

        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
            if (!(expected_token & TOKEN_NUMBER))
            {
                return SORT_FAILURE;
            }

            expected_token = TOKEN_NUMBER | TOKEN_BREAK;
            break;

        default:
            return SORT_FAILURE;
        }
    }

    memory->length = length;
    return SORT_SUCCESS;
}

void sort_parse_numbers(const char* array_string, sort_memory_t* memory)
{
    int buffer_index = 0;
    unsigned long long array_index = 0;
    char num_buffer[SORT_BUFF_SIZE] = {0};
    unsigned long max_number = 0;
    char token;

    while ((token = *(array_string++)))
    {
        switch (token)
        {
        case ',': case '\n':
            memory->array[array_index] = strtoul(num_buffer, NULL,
                SORT_PARSE_BASE);

            if (memory->array[array_index] > max_number)
            {
                max_number = memory->array[array_index];
            }

            memset(num_buffer, 0, buffer_index + 1);
            buffer_index = 0;
            ++array_index;
            break;

        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
            num_buffer[buffer_index] = token;
            ++buffer_index;
            break;
        }
    }

    /* Determine the max number of bits to sort */
    for (; max_number > 0; max_number >>= 1)
    {
        ++memory->max_bits;
    }
}
