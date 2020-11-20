#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tokens.h"

// load file into memory
const char **load_file(FILE *file)
{
    // temp pointers to use when reallocating
    char *tmp_line = NULL;
    char **tmp_lines = NULL;

    const size_t baseline_buffer_length = 2 << 11; // 4096

    // fixed-size buffer for use with fgets
    char buffer[baseline_buffer_length + 1];
    // properly initialize the string
    buffer[0] = '\0';

    // the full line to be stored
    char *line = NULL;
    size_t line_size = 0;

    const size_t baseline_line_array_length = 2 << 11;
    char **lines = malloc(sizeof(char *) * baseline_line_array_length);
    size_t lines_size = baseline_line_array_length;

    size_t num_lines_stored = 0;

    if (lines == NULL)
    {
        fprintf(stderr, "Memory Error\n");
        exit(1);
    }

    while (fgets(buffer, sizeof buffer, file) != NULL)
    {
        tmp_line = realloc(line, line_size + strlen(buffer) + 1); // account for terminator
        if (tmp_line == NULL)
        {
            // TODO: free lines array elements as well
            free(lines);
            free(line);
            exit(1);
        }
        else if (line != tmp_line)
        {
            line = tmp_line;
        }
        tmp_line = NULL;
        // append current buffer to the line
        strcat(line, buffer);
        line_size = strlen(line);

        char *newline = strchr(line, '\n');
        // no newline in the current buffer
        if (newline == NULL)
        {
            // last line of file fully read (no newline at EOF)
            if (feof(file))
            {
                // append current line to array
                lines[num_lines_stored++] = line;
                line = NULL;
            }
        }
        else // line fully read and newline is at end
        {
            // replace newline with null char
            *newline = '\0';
            newline = NULL;

            // store the buffer in the array
            lines[num_lines_stored++] = line;
            line = NULL;

            // clear the buffer
            buffer[0] = '\0';

            // reallocate the lines array if necessary
            if (num_lines_stored == lines_size)
            {
                lines_size += baseline_line_array_length;
                tmp_lines = realloc(lines, sizeof(char *) * lines_size);

                if (tmp_lines == NULL) // realloc failed
                {
                    // TODO: need to free the lines array elements as well
                    free(lines);
                    free(buffer);
                    exit(1);
                }
                else if (lines != tmp_lines) // old value of lines was freed
                {
                    lines = tmp_lines;
                }
                tmp_lines = NULL; // avoid dangling pointer
            }
        }
    }

    // fgets returned null pointer but not at EOF
    if (!feof(file))
    {
        fprintf(stderr, "Error reading from source file\n");
        exit(1);
    }

    return (const char **)lines;

}

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
        const char **lines = load_file(file);
        printf("%s\n", lines[0]);
        printf("%s\n", lines[1]);
        fclose(file);
    }
    return 0;
}
