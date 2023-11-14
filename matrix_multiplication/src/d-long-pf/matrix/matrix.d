module matrix;

import std.conv;
import std.stdio;
import std.range;
import std.exception;
import std.parallelism;

import matrix_utils;

/**
 * Represents a matrix
 */
struct Matrix
{
    long[] array;   ///< Contains the elements row by row
    uint rows;      ///< Rows of the matrix
    uint cols;      ///< Cols of the matrix
}

/**
 * Exception for parsing errors
 */
class MatrixParserException : Exception
{
    this (string msg, string file = __FILE__, size_t line = __LINE__)
    {
        super(msg, file, line);
    }
}

/**
 * Exception for matrix dimension errors
 */
class MatrixDimensionException : Exception
{
    this (string msg, string file = __FILE__, size_t line = __LINE__)
    {
        super(msg, file, line);
    }
}

/**
 * Creates a matrix from string
 *
 * The function may change the matrix attributes, even on failure
 *
 * @param matrix_as_string String representation of the matrix
 * @param matrix Resulting matrix
 */
void matrixFromString(const ref string matrixAsString, ref Matrix matrix)
{
    matrixCheckAndParseDimensions(matrixAsString, matrix);
    matrixParseNumbers(matrixAsString, matrix);
}

/**
 * Performs a matrix multiplication
 *
 * The function fails, if the dimensions of the matrices don't match
 *
 * @param matrix1 First matrix
 * @param matrix2 Second matrix
 * @param result Result of matrix1 * matrix2
 * @throws MatrixDimensionException if dimensions don't match
 */
void matrixMult(const ref Matrix matrix1, const ref Matrix matrix2,
    ref Matrix result)
{
    if (matrix1.cols != matrix2.rows)
    {
        throw new MatrixDimensionException("Matrix dimensions dont match");
    }

    result.rows = matrix1.rows;
    result.cols = matrix2.cols;

    result.array = new long[result.rows * result.cols];
    result.array[] = 0; // Initialize array

    for (int i = 0; i < result.rows; ++i)
    {
        for (int j = 0; j < result.cols; ++j)
        {
            for (int k = 0; k < matrix2.rows; ++k)
            {
                result.array[matrix1dIndex(i, j, result)] +=
                    matrix1.array[matrix1dIndex(i, k, matrix1)]
                    * matrix2.array[matrix1dIndex(k, j, matrix2)];
            }
        }
    }
}

/**
 * Performs a parallel matrix multiplication
 *
 * The function fails, if the dimensions of the matrices don't match
 *
 * @param matrix1 First matrix
 * @param matrix2 Second matrix
 * @param result Result of matrix1 * matrix2
 * @param threadCount Worker thread count
 * @throws MatrixDimensionException if dimensions don't match
 */
void matrixMultParallel(const ref Matrix matrix1, const ref Matrix matrix2,
    ref Matrix result, uint threadCount)
{
    if (matrix1.cols != matrix2.rows)
    {
        throw new MatrixDimensionException("Matrix dimensions dont match");
    }

    result.rows = matrix1.rows;
    result.cols = matrix2.cols;

    result.array = new long[result.rows * result.cols];
    result.array[] = 0; // Initialize array

    defaultPoolThreads(threadCount - 1); // Main thread also works

    for (int i = 0; i < result.rows; ++i)
    {
        foreach (j; parallel(iota(result.cols)))
        {
            for (int k = 0; k < matrix2.rows; ++k)
            {
                result.array[matrix1dIndex(i, j, result)] +=
                    matrix1.array[matrix1dIndex(i, k, matrix1)]
                    * matrix2.array[matrix1dIndex(k, j, matrix2)];
            }
        }
    }
}

/**
 * Writes the matrix matrix to the file with name filename
 *
 * @param matrix Matrix to print
 * @param filename Name of the file
 */
void matrixWriteFile(const ref Matrix matrix, const ref string filename)
 {
     File file = File(filename, "w");

     file.write("[");

     for (int row = 0; row < matrix.rows; ++row)
     {
         file.write("[");

         for (int col = 0; col < matrix.cols; ++col)
         {
             file.write(matrix.array[matrix1dIndex(row, col, matrix)]);

             if (col != matrix.cols - 1)
             {
                 file.write(",");
             }
         }

         file.write("]");

         if (row != matrix.rows - 1)
         {
             file.write(",");
         }
     }

     file.writeln("]");

     // File is closed automatically here
 }
