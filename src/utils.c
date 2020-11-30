#include <string.h>
#include "utils.h"

static char *read_line(char **line, size_t *size, FILE *file)
{
    // temp pointer to use when reallocating
    char *tmp_line = NULL;

    // TODO: fix bug where not all lines are read when *line requires realloc
    const size_t baseline_buffer_length = 1 << 8; // 256

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
            if (*line == NULL)
            {
                tmp_line = calloc(strlen(buffer) + 1, 1);
            }
            else
            {
                tmp_line = realloc(*line, *size + strlen(buffer) + 1); // account for terminator
            }

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

static bf_source_object *create_source_object(void)
{
    bf_source_object *source = malloc(sizeof(bf_source_object));

    if (source == NULL)
    {
        return NULL;
    }

    source->lines = NULL;
    source->num_lines = 0;

    return source;
}

void free_source_object(bf_source_object *source)
{
    for (size_t i = 0; i < source->num_lines; ++i)
    {
        free((source->lines)[i]);
    }
    free(source->lines);
    free(source);
}

// load file into memory
bf_source_object *load_file(FILE *file)
{
    // temp pointer to use when reallocating
    char **tmp_lines = NULL;

    const size_t baseline_line_array_length = 1 << 8;

    size_t lines_size = 0;

    // create the source file object
    bf_source_object *source = create_source_object();

    if (source == NULL)
    {
        return NULL;
    }
    source->lines = malloc(sizeof(char *) * baseline_line_array_length);

    if (source->lines == NULL)
    {
        free_source_object(source);
        return NULL;
    }
    lines_size += baseline_line_array_length;

    size_t *line_size = malloc(sizeof(size_t));
    char **line = malloc(sizeof(char *));
    if (line == NULL || line_size == NULL)
    {
        free(line);
        free(line_size);
        free_source_object(source);
        return NULL;
    }
    *line_size = 0;
    *line = NULL;

    while (read_line(line, line_size, file) != NULL)
    {
        // store the line in the array
        (source->lines)[(source->num_lines)++] = *line;

        // create a new line
        char **tmp_line = realloc(line, sizeof(char *));
        if (tmp_line == NULL)
        {
            free(line);
            free(line_size);
            free_source_object(source);
            return NULL;
        }
        else if (tmp_line != line)
        {
            line = tmp_line;
        }
        tmp_line = NULL;
        *line = NULL;
        *line_size = 0;
        // reallocate the lines array if necessary
        if (lines_size == source->num_lines)
        {
            lines_size += baseline_line_array_length;
            tmp_lines = realloc(source->lines, sizeof(char *) * (lines_size));

            if (tmp_lines == NULL) // realloc failed
            {
                free_source_object(source);
                return NULL;
            }
            else if (source->lines != tmp_lines) // old value of lines was freed
            {
                source->lines = tmp_lines;
            }
            tmp_lines = NULL; // avoid dangling pointer
        }
    }

    free(line_size);
    free(*line);
    free(line);

    // read_line returned null pointer but not at EOF
    if (!feof(file))
    {
        free_source_object(source);
        return NULL;
    }

    // reallocate the lines array to contain only the number of lines stored
    tmp_lines = realloc(source->lines, sizeof(char *) * (source->num_lines));
    if (tmp_lines == NULL)
    {
        free_source_object(source);
        return NULL;
    }
    else if (source->lines != tmp_lines)
    {
        source->lines = tmp_lines;
    }
    tmp_lines = NULL;

    return source;
}
