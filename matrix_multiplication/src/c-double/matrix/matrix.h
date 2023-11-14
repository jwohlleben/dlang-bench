#ifndef MATRIX_H
#define MATRIX_H

/* Defines for matrix return codes */
#define MATRIX_SUCCESS      0x00 ///< Success
#define MATRIX_PAR_ERROR    0x01 ///< Parsing error
#define MATRIX_MEM_ERROR    0x02 ///< Memory allocation error
#define MATRIX_DIM_ERROR    0x03 ///< Matrix dimensions don't match
#define MATRIX_FILE_ERROR   0x04 ///< Could not write to file

/**
 * Represents a matrix
 */
typedef struct _matrix_t
{
    double* array;      ///< Contains the elements row by row
    unsigned int rows;  ///< Rows of the matrix
    unsigned int cols;  ///< Cols of the matrix
} matrix_t;

/**
 * Creates a matrix from string
 *
 * The function may change the matrix attributes, even on failure
 *
 * @param matrix_as_string String representation of the matrix
 * @param matrix Resulting matrix
 * @return MATRIX_SUCCESS, if successful
 */
int matrix_from_string(const char* matrix_as_string, matrix_t* matrix);

/**
 * Performs a matrix multiplication
 *
 * The function fails, if the dimensions of the matrices don't match
 *
 * @param matrix1 First matrix
 * @param matrix2 Second matrix
 * @param result Result of matrix1 * matrix2
 * @return MATRIX_SUCCESS, if successful
 */
int matrix_mult(const matrix_t* matrix1, const matrix_t* matrix2,
    matrix_t* result);

/**
 * Performs a parallel matrix multiplication
 *
 * The function fails, if the dimensions of the matrices don't match
 *
 * @param matrix1 First matrix
 * @param matrix2 Second matrix
 * @param result Result of matrix1 * matrix2
 * @param thread_count Worker thread count
 * @return MATRIX_SUCCESS, if successful
 */
int matrix_mult_parallel(const matrix_t* matrix1, const matrix_t* matrix2,
    matrix_t* result, unsigned int thread_count);

/**
 * Writes the matrix matrix to the file with name filename
 *
 * @param matrix Matrix to print
 * @param filename Name of the file
 * @return MATRIX_SUCCESS, if successful
 */
int matrix_write_file(const matrix_t* matrix, const char* filename);

#endif
