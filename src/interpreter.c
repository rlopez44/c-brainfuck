#include "interpreter.h"

bf_token *verify_loop_integrity(bf_tokens *tokens)
{
    /* the loop token index stack. For now a constant size, but
     * could maybe be reimplemented to be dynamically resized
     */
    size_t stack[tokens->num_tokens];
    size_t stack_ptr = -1;

    // iterate over the source tokens, tracking loops as we go
    for (size_t i = 0; i < tokens->num_tokens; ++i)
    {
        bf_token *curr_token = (tokens->tokens)[i];
        switch (curr_token->token_type)
        {
            case L_BRACKET:
                // push the token index onto the token stack
                stack[++stack_ptr] = i;
                break;

            case R_BRACKET:
                /* attempt to pop a left bracket off the stack. If the stack is empty,
                 * then we have an unmatched bracket and we return this token
                 */
                if (stack_ptr == -1)
                {
                    return curr_token;
                }
                --stack_ptr;
                break;

            // don't care about other tokens. Only checking loops
            default:
                break;
        }
    }

    // check if any unmatched open brackets are left
    if (stack_ptr != -1)
    {
        /* one or more unmatched open brackets.
         * send back the first unmatched token
         */
        return (tokens->tokens)[stack[0]];
    }

    // no unmatched tokens found
    return NULL;
}
