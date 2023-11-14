module sort_utils;

import std.conv;

import my_sort;

/* Defines for parsing */
enum TOKENS
{
    BREAK   = 0x01, ///< ',' or ' ' or '\n' is expected
    NUMBER  = 0x02, ///< A number is expected
}

enum SORT_BUFF_SIZE = 0x20; ///< Buffer size for converting chars to nums

/**
 * Parses a given array string and returns the array length
 *
 * @param arrayString String representation of the array
 * @throws SortParserException, if the string couldn't be parsed
 * @return Length of the array
 */
ulong sortCheckAndParseLength(const ref string arrayString)
{
    TOKENS expectedToken = TOKENS.NUMBER;
    ulong length = 0;

    foreach (token; arrayString)
    {
        switch (token)
        {
        case ',': case '\n':
            if (!(expectedToken & TOKENS.BREAK))
            {
                throw new SortParserException("Did not expect ,");
            }

            ++length;
            expectedToken = TOKENS.NUMBER;
            break;

        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
            if (!(expectedToken & TOKENS.NUMBER))
            {
                throw new SortParserException("Did not expect number");
            }

            expectedToken = TOKENS.NUMBER | TOKENS.BREAK;
            break;

        default:
            throw new SortParserException("Unknown character");
        }
    }

    return length;
}

/**
 * Parses the arrayString and fills the numbers array. Needs a already
 * initialized numbers array
 *
 * @param arrayString Array as string
 * @param memory The memory for the numbers array
 */
void sortParseNumbers(const ref string arrayString, ref SortMemory memory)
{
    int bufferIndex = 0;
    uint maxNumber = 0;
    ulong arrayIndex = 0;
    char[SORT_BUFF_SIZE] numBuffer;

    foreach (token; arrayString)
    {
        switch (token)
        {
        case ',': case '\n':
            memory.array[arrayIndex] = to!uint(numBuffer[0..bufferIndex]);

            if (memory.array[arrayIndex] > maxNumber)
            {
                maxNumber = memory.array[arrayIndex];
            }

            bufferIndex = 0;
            ++arrayIndex;
            break;

        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
            numBuffer[bufferIndex] = token;
            ++bufferIndex;
            break;
        
        default:
            break;
        }
    }

    /* Determine the max number of bits to sort */
    for (; maxNumber > 0; maxNumber >>= 1)
    {
        ++memory.maxBits;
    }
}
