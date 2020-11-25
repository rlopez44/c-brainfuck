#ifndef UTILS_H_
#define UTILS_H_

#include <stdio.h>
#include <stdlib.h>

typedef struct bf_source_object {
    char **lines;
    size_t num_lines;
} bf_source_object;

bf_source_object *load_file(FILE *file);

void free_source_object(bf_source_object *source);

#endif
