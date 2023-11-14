#ifndef SORT_UTILS_H
#define SORT_UTILS_H

#include "sort.h"

/* Defines for parsing */
#define TOKEN_BREAK     0x01 ///< ',' or ' ' or '\n' is expected
#define TOKEN_NUMBER    0x02 ///< A number is expected
#define SORT_PARSE_BASE 0x0A ///< Use base 10 for converting numbers

/* Defines for sizes */
#define SORT_BUFF_SIZE  0x20 ///< Buffer size for converting chars to nums

/**
 * Checks the array_string and sets the memory length
 *
 * @param array_string Array as string
 * @param memory The memory to set the length
 * @return SORT_SUCCESS, if successful
 */
int sort_check_and_parse_length(const char* array_string,
    sort_memory_t* memory);

/**
 * Parses the array_string and fills the numbers array. Needs
 * the memory length parameter and a malloc'd array.
 *
 * @param array_string Array as string
 * @param memory The memory for the numbers array
 */
void sort_parse_numbers(const char* array_string, sort_memory_t* memory);

#endif
