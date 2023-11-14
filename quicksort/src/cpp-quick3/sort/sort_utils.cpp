#include "sort_utils.hpp"

#include <memory>
#include <cstdlib>
#include <cstring>

#include "sort.hpp"

unsigned long long sort_check_and_parse_length(
    const std::shared_ptr<char> array_string)
{
    const char* string = array_string.get();

    char token;
    char expected_token = TOKEN_NUMBER;
    unsigned long long length = 0;

    while ((token = *(string++)))
    {
        switch (token)
        {
        case ',': case '\n':
            if (!(expected_token & TOKEN_BREAK))
            {
                throw sort_parser_exception();
            }

            ++length;
            expected_token = TOKEN_NUMBER;
            break;

        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
            if (!(expected_token & TOKEN_NUMBER))
            {
                throw sort_parser_exception();
            }

            expected_token = TOKEN_NUMBER | TOKEN_BREAK;
            break;

        default:
            throw sort_parser_exception();
        }
    }

    return length;
}

std::vector<unsigned long> sort_parse_numbers(unsigned long long vector_size,
    const std::shared_ptr<char> array_string)
{
    const char* string = array_string.get();

    std::vector<unsigned long> vector;
    vector.resize(vector_size);

    unsigned long long vector_index = 0;
    int buffer_index = 0;
    char num_buffer[SORT_BUFF_SIZE] = {0};
    char token;

    while ((token = *(string++)))
    {
        switch (token)
        {
        case ',': case '\n':
            vector[vector_index] = strtoul(num_buffer, NULL, SORT_PARSE_BASE);
            memset(num_buffer, 0, buffer_index + 1);
            buffer_index = 0;
            ++vector_index;
            break;

        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
            num_buffer[buffer_index] = token;
            ++buffer_index;
            break;
        }
    }

    return vector;
}
