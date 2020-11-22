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

        char ***lines = malloc(sizeof(char **));
        *lines = NULL;
        size_t *lines_size = malloc(sizeof(size_t));
        *lines_size = 0;
        if (load_file(lines, lines_size, file)  == NULL)
        {
            fclose(file);
            // TODO: free line array elements as well
            free(lines);
            free(lines_size);
            fprintf(stderr, "Error: Failed to read from source file\n");
            return 1;
        }
        for (size_t i = 0; i < *lines_size; ++i)
        {
            printf("%s\n", (*lines)[i]);
        }
        fclose(file);
    }
    return 0;
}
