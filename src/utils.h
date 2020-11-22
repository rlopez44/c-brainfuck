#ifndef UTILS_H_
#define UTILS_H_

#include <stdio.h>

char *read_line(char **line, size_t *size, FILE *file);
char **load_file(char **lines[], size_t *lines_size, FILE *file);

#endif
