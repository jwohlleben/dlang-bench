#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFF_SIZE 32

/**
 * Generates a random number in [min;max]
 *
 * @param min Minimum number
 * @param max Maximum number
 * @return A random number
 */
static inline double random_number(double min, double max)
{
    double scale = (double) rand() / RAND_MAX;
    return min + scale * (max - min);
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
    if (argc != 6)
    {
        printf("Usage: %s rows cols min_num max_num filename\n", argv[0]);
        return EXIT_FAILURE;
    }

    const int rows = atoi(argv[1]);
    const int cols = atoi(argv[2]);
    const double min_num = atof(argv[3]);
    const double max_num = atof(argv[4]);
    const char* filename = argv[5];

    char buffer[BUFF_SIZE] = {0};

    time_t t;
    srand((unsigned) time(&t));

    FILE *fp = fopen(filename, "w");

    fputc('[', fp);
    for (int row = 0; row < rows; ++row)
    {
        fputc('[', fp);

        for (int col = 0; col < cols; ++col)
        {
            sprintf(buffer, "%lf", random_number(min_num, max_num));
            fputs(buffer, fp);
            memset(buffer, 0, BUFF_SIZE);
            if (col != cols - 1)
            {
                fputc(',', fp);
            }
        }

        fputc(']', fp);
        if (row != rows - 1)
        {
            fputc(',', fp);
        }
    }

    fputs("]\n", fp);
    fclose(fp);

    return EXIT_SUCCESS;
}
