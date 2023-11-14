#ifndef SORT_UTILS_HPP
#define SORT_UTILS_HPP

#include <memory>
#include <vector>

/* Defines for parsing */
#define TOKEN_BREAK     0x01 ///< ',' or ' ' or '\n' is expected
#define TOKEN_NUMBER    0x02 ///< A number is expected
#define SORT_PARSE_BASE 0x0A ///< Use base 10 for converting numbers

/* Defines for sizes */
#define SORT_BUFF_SIZE  0x20 ///< Buffer size for converting chars to nums

/**
 * Checks the array_string and returns the array length
 *
 * @param array_string Array as string
 * @throws sort_parser_exception, if the string couldn't be parsed
 * @return array length
 */
unsigned long long sort_check_and_parse_length(
    const std::shared_ptr<char> array_string);

/**
 * Parses the array_string and creates a numbers vector
 *
 * @param vector_size The size of the created vector
 * @param array_string Array as string
 * @return Numbers vector
 */
std::vector<unsigned long> sort_parse_numbers(unsigned long long vector_size,
    const std::shared_ptr<char> array_string);

#endif
