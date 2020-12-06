#ifndef TOKENIZE_H_
#define TOKENIZE_H_
#include "utils.h"

enum TOKENS
{
    RIGHT_SHIFT,
    LEFT_SHIFT,
    PLUS,
    MINUS,
    DOT,
    COMMA,
    L_BRACKET,
    R_BRACKET,
};

typedef struct bf_token {
    int token_type;
    size_t lineno;
    size_t col;
} bf_token;

typedef struct bf_tokens {
    bf_token **tokens;
    size_t num_tokens;
} bf_tokens;

bf_tokens *tokenize(bf_source_object *source);

void free_tokens_object(bf_tokens *tokens);

#endif
