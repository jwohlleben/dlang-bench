module matrix;

import core.thread;

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
    double[][] array; ///< Contains the elements row by row
    uint rows;        ///< Rows of the matrix
    uint cols;        ///< Cols of the matrix
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

    result.array = new double[][](result.rows, result.cols);

    /* Initialize array */
    for (int i = 0; i < result.rows; ++i)
    {
        for (int j = 0; j < result.cols; ++j)
        {
            result.array[i][j] = 0;
        }
    }

    for (int i = 0; i < result.rows; ++i)
    {
        for (int j = 0; j < result.cols; ++j)
        {
            for (int k = 0; k < matrix2.rows; ++k)
            {
                result.array[i][j] += matrix1.array[i][k] * matrix2.array[k][j];
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

    uint arrayLength = result.rows * result.cols;

    result.array = new double[][](result.rows, result.cols);

    /* Initialize array */
    for (int i = 0; i < result.rows; ++i)
    {
        for (int j = 0; j < result.cols; ++j)
        {
            result.array[i][j] = 0;
        }
    }

    uint indizesPerThread = to!uint(arrayLength / threadCount);
    uint remainingIndizes = arrayLength % threadCount;
    uint indexOffset = 0;

    uint mainThreadStartIndex = 0 * indizesPerThread + indexOffset;
    uint mainThreadEndIndex = 1 * indizesPerThread + indexOffset;

    if (remainingIndizes != 0)
    {
        ++mainThreadEndIndex;
        ++indexOffset;
        --remainingIndizes;
    }

    uint mainThreadStartRow = matrix2dIndexRow(mainThreadStartIndex, result);
    uint mainThreadStartCol = matrix2dIndexCol(mainThreadStartIndex, result);
    uint mainThreadEndRow = matrix2dIndexRow(mainThreadEndIndex, result);
    uint mainThreadEndCol = matrix2dIndexCol(mainThreadEndIndex, result);

    if (mainThreadEndCol == 0)
    {
        --mainThreadEndRow;
        mainThreadEndCol = result.cols - 1;
    }
    else
    {
        --mainThreadEndCol;
    }

    /* Distribute target indices fairly across other threads*/
    for (int i = 1; i < threadCount; ++i)
    {
        uint startIndex = i * indizesPerThread + indexOffset;
        uint endIndex = (i + 1) * indizesPerThread + indexOffset;

        if (remainingIndizes != 0)
        {
            ++endIndex;
            ++indexOffset;
            --remainingIndizes;
        }

        int startRow = matrix2dIndexRow(startIndex, result);
        int startCol = matrix2dIndexCol(startIndex, result);
        int endRow = matrix2dIndexRow(endIndex, result);
        int endCol = matrix2dIndexCol(endIndex, result);

        /* Adjust endRow and endCol, if end is reached */
        if (endCol == 0)
        {
            --endRow;
            endCol = result.cols - 1;
        }
        else
        {
            --endCol;
        }

        auto task = task!matrixMultWorkerThread(matrix1, matrix2, result,
            startRow, endRow, startCol, endCol);

        task.executeInNewThread();
    }

    // Main thread also calculates
    matrixMultWorkerThread(matrix1, matrix2, result, mainThreadStartRow,
        mainThreadEndRow, mainThreadStartCol, mainThreadEndCol);

    thread_joinAll(); // Wait for threads to finish
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
            file.write(matrix.array[row][col]);

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

/**
 * Performs a matrix multiplication in a worker thread
 *
 * @param matrix1 First matrix
 * @param matrix2 Second matrix
 * @param result Result of matrix1 * matrix2
 * @param startRow Start row of the multiplication
 * @param endtRow End row of the multiplication
 * @param startCol Start column of the multiplication
 * @param endCol End column of the multiplication
 */
void matrixMultWorkerThread(const ref Matrix matrix1, const ref Matrix matrix2,
    ref Matrix result, int startRow, int endRow, int startCol, int endCol)
{
    /* Finish first row */
    if (startRow == endRow)
    {
        matrixMultRange(matrix1, matrix2, result, startRow, endRow, startCol,
            endCol);

        return;
    }
    else
    {
        matrixMultRange(matrix1, matrix2, result, startRow, startRow, startCol,
            to!int(result.cols) - 1);
    }

    /* Calculate all mid rows */
    matrixMultRange(matrix1, matrix2, result, startRow + 1, to!int(endRow) - 1,
        0, to!int(result.cols) - 1);

    /* Finish last row */
    matrixMultRange(matrix1, matrix2, result, endRow, endRow, 0, endCol);
}

/**
 * Performs a matrix multiplication in defined range
 *
 * @param matrix1 First matrix
 * @param matrix2 Second matrix
 * @param result Result of matrix1 * matrix2
 * @param startRow Start row of the multiplication
 * @param endtRow End row of the multiplication
 * @param startCol Start column of the multiplication
 * @param endCol End column of the multiplication
 */
void matrixMultRange(const ref Matrix matrix1, const ref Matrix matrix2,
    ref Matrix result, int startRow, int endRow, int startCol, int endCol)
{
    for (int i = startRow; i <= endRow; ++i)
    {
        for (int j = startCol; j <= endCol; ++j)
        {
            for (int k = 0; k < matrix2.rows; ++k)
            {
                result.array[i][j] += matrix1.array[i][k] * matrix2.array[k][j];
            }
        }
    }
}
