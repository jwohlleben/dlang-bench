#include <stdio.h>
#include <stdlib.h>

#include "file_utils.h"

char* read_file(const char* filename)
{
    /* Open a file */
    FILE* fp = fopen(filename, "r");

    if (fp == NULL)
    {
        return NULL;
    }

    /* Check file size */
    if (fseek(fp, 0L, SEEK_END))
    {
        fclose(fp);
        return NULL;
    }

    long int file_size = ftell(fp);

    if (file_size == -1L)
    {
        fclose(fp);
        return NULL;
    }

    if (fseek(fp, 0L, SEEK_SET))
    {
        fclose(fp);
        return NULL;
    }

    /* Read file into memory */
    char* str = (char*) malloc(sizeof(char) * (file_size + 1));

    if (str == NULL)
    {
        fclose(fp);
        return NULL;
    }

    if(!fread(str, file_size, 1, fp))
    {
        fclose(fp);
        free(str);
        return NULL;
    }

    fclose(fp);

    str[file_size] = 0; // String terminator

    return str;
}
