#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/random.h>

#define BYTE_COUNT 4
#define BUFF_SIZE 32

/**
 * Generates a random number in [0; 4,294,967,295]
 *
 * @param min Minimum number
 * @param max Maximum number
 * @return A random number
 */
static inline unsigned long random_number()
{
    unsigned char bytes[BYTE_COUNT];
    getrandom(&bytes, BYTE_COUNT, 0);
    unsigned long num = 0;

    for (int i = 0; i < BYTE_COUNT; ++i)
    {
        num |= bytes[i];

        if (i < BYTE_COUNT - 1)
        {
            num <<= 8;
        }
    }

    return num;
}

/**
 * Reads parameters from argv and creates a matrix file
 *
 * @param argc Argument count
 * @param argv Argument strings
 * @return EXIT_SUCCESS, if successful
 */
int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Usage: %s length filename\n", argv[0]);
        return EXIT_FAILURE;
    }

    const unsigned long long length = strtoull(argv[1], NULL, 10);
    const char* filename = argv[2];
    char buffer[BUFF_SIZE] = {0};

    FILE *fp = fopen(filename, "w");
    for (unsigned long long l = 0; l < length; ++l)
    {
        sprintf(buffer, "%lu", random_number());
        fputs(buffer, fp);
        memset(buffer, 0, BUFF_SIZE);

        if (l != length - 1)
        {
            fputc(',', fp);
        }
    }

    fputc('\n', fp);

    fclose(fp);
    return EXIT_SUCCESS;
}
