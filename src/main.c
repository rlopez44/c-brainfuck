#include <stdio.h>
#include <stdlib.h>
#include "tokenize.h"
#include "utils.h"
#include "interpreter.h"

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

        bf_token *unmatched_loop_token = verify_loop_integrity(tokens);
        if (unmatched_loop_token == NULL)
        {
            interpret(tokens);
        }
        else
        {
            size_t error_line = unmatched_loop_token->lineno;
            size_t error_col = unmatched_loop_token->col;
            printf("Syntax error (unmatched bracket) on line #%zu:\n", error_line);
            printf("%s\n", (source->lines)[error_line - 1]);
            for (size_t i = 1; i < error_col; ++i)
            {
                putchar(' ');
            }
            printf("^\n");

            free_source_object(source);
            free_tokens_object(tokens);
            return 1;
        }

        free_source_object(source);
        free_tokens_object(tokens);
    }
    return 0;
}
