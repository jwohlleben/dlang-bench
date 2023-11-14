module matrix_utils;

import std.conv;

import matrix;

/* Defines for parsing */
enum TOKENS
{
    NONE          = 0x01, ///< Nothing is expected
    SIGN          = 0x02, ///< '+' or '-' sign is expected
    COLON         = 0x04, ///< '.' is expected
    COMMA         = 0x08, ///< ',' is expected
    NUMBER        = 0x10, ///< A number is expected
    BRACKET_OPEN  = 0x20, ///< '[' is expected
    BRACKET_CLOSE = 0x40, ///< ']' is expected
}

enum MATRIX_BUFF_SIZE = 0x20; ///< Buffer size for converting chars to nums

/**
 * Calculates the 2D row index from 1D index values
 *
 * @param index1d 1D index
 * @param matrix Matrix for indexing
 * @return 2D row index
 */
uint matrix2dIndexRow(uint index1d, const ref Matrix matrix)
{
    return cast(uint)index1d / matrix.cols;
}

/**
 * Calculates the 2D colum index from 1D index values
 *
 * @param index1d 1D index
 * @param matrix Matrix for indexing
 * @return 2D colum index
 */
uint matrix2dIndexCol(uint index1d, const ref Matrix matrix)
{
    return index1d % matrix.cols;
}

/**
 * Parses a given matrix string and sets the dimensions of the resulting matrix
 *
 * The function may change the matrix attributes, even on failure
 *
 * @param matrixAsString String representation of the matrix
 * @param matrix Target matrix
 * @throws MatrixParserException if matrixAsString couldn't be parsed
 */
void matrixCheckAndParseDimensions(const string matrixAsString,
    ref Matrix matrix)
{
    uint rows = 0;
    uint cols = 0;
    int innerCommas = 0;
    int outerCommas = 0;
    int bracketCount = 0;
    bool seenColon = false;
    TOKENS expectedToken = TOKENS.BRACKET_OPEN;

    foreach (token; matrixAsString)
    {
        switch (token)
        {
        case '[':
            if (!(expectedToken & TOKENS.BRACKET_OPEN))
            {
                throw new MatrixParserException("Did not expect [");
            }

            if (bracketCount < 1)
            {
                expectedToken = TOKENS.BRACKET_OPEN;
            }
            else
            {
                expectedToken = TOKENS.SIGN | TOKENS.NUMBER;
            }

            ++bracketCount;
            break;

        case ']':
            if (!(expectedToken & TOKENS.BRACKET_CLOSE))
            {
                throw new MatrixParserException("Did not expect ]");
            }
            if (bracketCount == 2)
            {
                if (cols == 0)
                {
                    cols = innerCommas + 1;
                }
                else
                {
                    if (cols != (innerCommas + 1))
                    {
                        throw new MatrixParserException("Missing colums");
                    }
                }

                innerCommas = 0;
                expectedToken = TOKENS.BRACKET_CLOSE | TOKENS.COMMA;
            }
            else if (bracketCount == 1)
            {
                rows = outerCommas + 1;
                expectedToken = TOKENS.NONE;
            }

            --bracketCount;
            break;

        case ',':
            if (!(expectedToken & TOKENS.COMMA))
            {
                throw new MatrixParserException("Did not expect ,");
            }

            switch (bracketCount)
            {
            case 1:
                ++outerCommas;
                expectedToken = TOKENS.BRACKET_OPEN;
                break;
            case 2:
                ++innerCommas;
                expectedToken = TOKENS.SIGN | TOKENS.NUMBER;
                break;
            default:
                assert(0);
            }

            seenColon = false;
            break;

        case '+': case '-':
            if (!(expectedToken & TOKENS.SIGN))
            {
                throw new MatrixParserException("Did not expect sign");
            }

            expectedToken = TOKENS.NUMBER;
            break;

        case '.':
            if (!(expectedToken & TOKENS.COLON))
            {
                throw new MatrixParserException("Did not expect .");
            }

            expectedToken = TOKENS.NUMBER;
            seenColon = true;
            break;

        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
            if (!(expectedToken & TOKENS.NUMBER))
            {
                throw new MatrixParserException("Did not expect number");
            }

            expectedToken = TOKENS.NUMBER | TOKENS.COMMA |
                TOKENS.BRACKET_CLOSE;

            if (!seenColon)
            {
                expectedToken |= TOKENS.COLON;
            }

            break;

        case ' ': case '\n':
            break;

        default:
            throw new MatrixParserException("Unknown character");
        }

        matrix.rows = rows;
        matrix.cols = cols;
    }
}

/**
 * Parses the numbers of a given matrix string
 *
 * @param matrixAsString String representation of the matrix
 * @param matrix Target matrix
 */
void matrixParseNumbers(const ref string matrixAsString, ref Matrix matrix)
{
    matrix.array = new long[][](matrix.rows, matrix.cols);

    bool numAvailable = false;
    int matrixIndex = 0;
    int bufferIndex = 0;
    char[MATRIX_BUFF_SIZE] numBuffer;

    foreach (token; matrixAsString)
    {
        switch (token)
        {
        case '+': case '-': case '.':
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
            numAvailable = true;

            // Fill buffer with tokens
            numBuffer[bufferIndex] = token;
            ++bufferIndex;
            break;

        case ',': case ']':
            if (!numAvailable)
            {
                break;
            }
            int row = matrix2dIndexRow(matrixIndex, matrix);
            int col = matrix2dIndexCol(matrixIndex, matrix);
            numAvailable = false;
            matrix.array[row][col] = to!long(numBuffer[0..bufferIndex]);
            bufferIndex = 0;
            ++matrixIndex;
            break;

        default:
            break;
        }
    }
}
