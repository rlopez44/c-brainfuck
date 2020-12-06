#include <stdio.h>
#include <stdlib.h>
#include "tokenize.h"
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

        bf_tokens *tokens = tokenize(source);
        if (tokens == NULL)
        {
            free_source_object(source);
            fprintf(stderr, "Memory Error\n");
            return 1;
        }

        for (size_t i = 0; i < tokens->num_tokens; ++i)
        {
            int token_type = (tokens->tokens)[i]->token_type;
            size_t lineno = (tokens->tokens)[i]->lineno;
            size_t col = (tokens->tokens)[i]->col;
            printf("Token Type: %d, LineNo: %zu, Col: %zu\n", token_type, lineno, col);
        }
        free_source_object(source);
        free_tokens_object(tokens);
    }
    return 0;
}
