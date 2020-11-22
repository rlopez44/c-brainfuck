#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "utils.h"

char *read_line(char **line, size_t *size, FILE *file)
{
    // temp pointer to use when reallocating
    char *tmp_line = NULL;

    const size_t baseline_buffer_length = 1 << 8; // 4096

    // fixed-size buffer for use with fgets
    char buffer[baseline_buffer_length + 1];
    // properly initialize the string
    buffer[0] = '\0';

    // the location of the newline
    char *newline = NULL;

    while (1)
    {
        // error reading from file
        if (fgets(buffer, sizeof buffer, file) == NULL)
        {
            return NULL;
        }

        // expand the line size if necessary
        if (*line == NULL || *size < strlen(*line) + strlen(buffer) + 1)
        {
            tmp_line = realloc(*line, *size + strlen(buffer) + 1); // account for terminator
            if (tmp_line == NULL)
            {
                return NULL;
            }
            else if (*line != tmp_line)
            {
                *line = tmp_line;
            }
            tmp_line = NULL;
        }

        // append current buffer to the line
        strcat(*line, buffer);
         *size = strlen(*line);

        newline = strchr(*line, '\n');
        // no newline in the current buffer
        if (newline == NULL)
        {
            // last line of file fully read (no newline at EOF)
            // otherwise, line not fully-read, must reallocate
            if (feof(file))
            {
                return *line;
            }
        }
        else // line fully read and newline is at end
        {
            // replace newline with null char
            *newline = '\0';
            newline = NULL;

            return *line;
        }

    }
}

// load file into memory
char ***load_file(char **lines[], size_t *lines_size, FILE *file)
{
    // temp pointer to use when reallocating
    char **tmp_lines = NULL;

    const size_t baseline_line_array_length = 1 << 8;

    size_t num_lines_stored = 0;

    // initial reallocation, if necessary
    if (!(*lines_size))
    {
        tmp_lines = realloc(*lines, sizeof(char *) * baseline_line_array_length);
        *lines_size = baseline_line_array_length;
        if (tmp_lines == NULL)
        {
            return NULL;
        }
        else if (*lines != tmp_lines)
        {
            *lines = tmp_lines;
        }
        tmp_lines = NULL;
    }

    size_t *line_size = malloc(sizeof(size_t));
    char **line = malloc(sizeof(char *));
    if (line == NULL || line_size == NULL)
    {
        return NULL;
    }
    *line_size = 0;
    *line = NULL;

    while (read_line(line, line_size, file) != NULL)
    {
        // store the line in the array
        (*lines)[num_lines_stored++] = *line;
        ++(*lines_size);

        // create a new line
        line = malloc(sizeof(char *));
        if (line == NULL)
        {
            return NULL;
        }
        *line = NULL;
        *line_size = 0;
        // reallocate the lines array if necessary
        if (num_lines_stored == *lines_size)
        {
            *lines_size += baseline_line_array_length;
            tmp_lines = realloc(*lines, sizeof(char *) * (*lines_size));

            if (tmp_lines == NULL) // realloc failed
            {
                return NULL;
            }
            else if (*lines != tmp_lines) // old value of lines was freed
            {
                *lines = tmp_lines;
            }
            tmp_lines = NULL; // avoid dangling pointer
        }
    }

    // read_line returned null pointer but not at EOF
    if (!feof(file))
    {
        return NULL;
    }

    // reallocate the lines array to contain only the number of lines stored
    tmp_lines = realloc(*lines, sizeof(char *) * num_lines_stored);
    if (tmp_lines == NULL)
    {
        return NULL;
    }
    else if (*lines != tmp_lines)
    {
        *lines = tmp_lines;
    }
    tmp_lines = NULL;
    *lines_size = num_lines_stored;

    return lines;

}
