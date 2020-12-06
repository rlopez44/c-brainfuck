#include <string.h>
#include "utils.h"
#include "tokenize.h"

static const char TOKEN_CHARS[] = {
    '>', // increment the data pointer
    '<', // decrement the data pointer
    '+', // increment the byte at the data pointer
    '-', // decrement the byte at the data pointer
    '.', // output the byte at the data pointer
    ',', // accept one byte of input, storing its value in the byte at the data pointer
    '[', // if the byte at the data pointer is nonzero, go to next command, else jump forward to matching ']'
    ']', // if the byte at the data pointer is zero, go to next command, else jump back to the matching '['
};

void free_tokens_object(bf_tokens *tokens)
{
    for (size_t i = 0; i < tokens->num_tokens; ++i)
    {
        free((tokens->tokens)[i]);
    }
    free(tokens->tokens);
    free(tokens);
}

static bf_token *create_token_object(int token_type, size_t lineno, size_t col)
{
    bf_token *token = malloc(sizeof(bf_token));
    if (token == NULL)
    {
        return NULL;
    }
    token->token_type = token_type;
    token->lineno = lineno;
    token->col = col;

    return token;
}

static bf_tokens *create_tokens_object(void)
{
    bf_tokens *tokens = malloc(sizeof(bf_tokens));
    if (tokens == NULL)
    {
        return NULL;
    }

    tokens->tokens = NULL;
    tokens->num_tokens = 0;

    return tokens;
}

static bf_token **extract_tokens(bf_tokens *tokens, char *line, size_t lineno)
{
    bf_token **tmp_token_arr = realloc(tokens->tokens, (tokens->num_tokens + strlen(line)) * sizeof(bf_token *));
    if (tmp_token_arr == NULL)
    {
        return NULL;
    }
    else if (tmp_token_arr != tokens->tokens)
    {
        tokens->tokens = tmp_token_arr;
    }
    tmp_token_arr = NULL;

    size_t curr_pos = tokens->num_tokens;
    for (size_t i = 0; i < strlen(line); ++i)
    {
        char *token_loc = strchr(TOKEN_CHARS, line[i]);
        if (token_loc != NULL) // all other chars are treated as comments
        {
            // index in TOKEN_CHARS gives correct TOKENS enum member
            int token_type = (int)(token_loc - TOKEN_CHARS);

            bf_token *tmp_token = create_token_object(token_type, lineno, i + 1);
            if (tmp_token == NULL)
            {
                return NULL;
            }

            (tokens->tokens)[curr_pos++] = tmp_token;
            tmp_token = NULL;
            ++(tokens->num_tokens);
        }
    }
    tmp_token_arr = realloc(tokens->tokens, (tokens->num_tokens) * sizeof(bf_token *));
    if (tmp_token_arr == NULL)
    {
        return NULL;
    }
    else if (tmp_token_arr != tokens->tokens)
    {
        tokens->tokens = tmp_token_arr;
    }
    tmp_token_arr = NULL;

    return tokens->tokens;
}

bf_tokens *tokenize(bf_source_object *source)
{
    bf_tokens *tokens = create_tokens_object();
    if (tokens == NULL)
    {
        return NULL;
    }

    bf_token **token_arr = NULL;
    for (size_t i = 0; i < source->num_lines; ++i)
    {
        // extract tokens from each line
        token_arr = extract_tokens(tokens, (source->lines)[i], i + 1);
        if (token_arr == NULL)
        {
            free_tokens_object(tokens);
            return NULL;
        }
    }

    return tokens;
}
