#ifndef FILE_UTILS_H
#define FILE_UTILS_H

/**
 * Reads a whole file into memory
 *
 * @param filename Name of the file
 * @return On success: Pointer to a char array representing the file
 *         contents. On error: NULL
 */
char* read_file(const char* filename);

#endif
