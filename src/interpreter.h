#ifndef PARSE_H_
#define PARSE_H_
#include "tokenize.h"

// verify that there are no unclosed loops in the source code
bf_token *verify_loop_integrity(bf_tokens *tokens);

void interpret(bf_tokens *tokens);

#endif
