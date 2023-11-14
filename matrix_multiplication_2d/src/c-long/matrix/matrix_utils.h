#ifndef MATRIX_UTILS_H
#define MATRIX_UTILS_H

#include "matrix.h"

/* Defines for parsing */
#define TOKEN_NONE          0x01 ///< Nothing is expected
#define TOKEN_SIGN          0x02 ///< '+' or '-' sign is expected
#define TOKEN_COLON         0x04 ///< '.' is expected
#define TOKEN_COMMA         0x08 ///< ',' is expected
#define TOKEN_NUMBER        0x10 ///< A number is expected
#define TOKEN_BRACKET_OPEN  0x20 ///< '[' is expected
#define TOKEN_BRACKET_CLOSE 0x40 ///< ']' is expected

/* Defines for sizes */
#define MATRIX_BUFF_SIZE    0x20 ///< Buffer size for converting chars to nums

/**
 * Is used for multithreaded matrix multiplication
 */
typedef struct _matrix_args_t
{
    const matrix_t* matrix1; ///< First matrix
    const matrix_t* matrix2; ///< Second matrix
    matrix_t* result;        ///< Result of matrix1 * matrix2
    unsigned int start_row;  ///< Start row index of calculation
    unsigned int start_col;  ///< Start colum index of calculation
    unsigned int end_row;    ///< End row index of calculation
    unsigned int end_col;    ///< End colum index of calculation
} matrix_args_t;

/**
 * Calculates the 2D row index from 1D index values
 *
 * @param index_1d 1D index
 * @param matrix Matrix for indexing
 * @return 2D row index
 */
static inline unsigned int matrix_2d_index_row(unsigned int index_1d,
    const matrix_t* matrix)
{
    return (unsigned int) index_1d / matrix->cols;
}

/**
 * Calculates the 2D colum index from 1D index values
 *
 * @param index_1d 1D index
 * @param matrix Matrix for indexing
 * @return 2D colum index
 */
static inline unsigned int matrix_2d_index_col(unsigned int index_1d,
    const matrix_t* matrix)
{
    return index_1d % matrix->cols;
}

/**
 * Works on a matrix index range for parallel matrix multiplication
 *
 * @param pthread_args The arguments of the function
 * @return NULL
 */
void* matrix_mult_worker_thread(void* pthread_args);

/**
 * Works on a matrix index range for parallel matrix multiplication
 *
 * @param args The arguments of the function
 * @param start_row The start row of the matrix multiplication
 * @param end_row The end row of the matrix multiplication
 * @param start_col The start column of the matrix multiplication
 * @param end_col The end column of the matrix multiplication
 */
void matrix_mult_worker_thread_range(matrix_args_t* args, int start_row, 
    int end_row, int start_col, int end_col); 

/**
 * Parses a given matrix string and sets the dimensions of the resulting matrix
 *
 * The function may change the matrix attributes, even on failure
 *
 * @param matrix_as_string String representation of the matrix
 * @param matrix Target matrix
 * @return MATRIX_SUCCESS, if successful
 */
int matrix_check_and_parse_dimensions(const char* matrix_as_string,
    matrix_t* matrix);

/**
 * Parses the numbers of a given matrix string
 *
 * @param matrix_as_string String representation of the matrix
 * @param matrix Target matrix
 * @return MATRIX_SUCCESS, if successful
 */
int matrix_parse_numbers(const char* matrix_as_string,
    matrix_t* matrix);

/**
 * Allocates memory for the matrix. Needs rows and cols
 *
 * @param matrix Matrix
 * @return MATRIX_SUCCESS, if successful
 */
int matrix_malloc(matrix_t* matrix);

/**
 * Allocates memory for the matrix. Needs rows and cols
 *
 * @param matrix Matrix
 * @return MATRIX_SUCCESS, if successful
 */
void matrix_cleanup(matrix_t* matrix);

#endif
