#include <stdio.h>
#include <stdlib.h>
#include "tokens.h"
#include "utils.h"

int main(int argc, const char *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Error: No source file specified\n");
        return 1;
    }
    else if (argc > 2)
    {
        fprintf(stderr, "Error: Too many arguments\n");
        return 1;
    }
    else
    {
        // read in file
        const char *file_path = argv[1];
        FILE *file = fopen(file_path, "r");
        if (file == NULL)
        {
            fprintf(stderr, "Error: Source file could not be opened\n");
            return 1;
        }

        bf_source_object *source = load_file(file);
        if (source == NULL)
        {
            fclose(file);
            fprintf(stderr, "Error: Failed to read from source file\n");
            return 1;
        }
        fclose(file);

        printf("Number of lines: %zu\n", source->num_lines);
        for (size_t i = 0; i < source->num_lines; ++i)
        {
            printf("%s\n", (source->lines)[i]);
        }

        free_source_object(source);
    }
    return 0;
}
