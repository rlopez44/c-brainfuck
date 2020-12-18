#include "interpreter.h"

bf_token *verify_loop_integrity(bf_tokens *tokens)
{
    /* the loop token index stack. For now a constant size, but
     * could maybe be reimplemented to be dynamically resized
     */
    size_t stack[tokens->num_tokens];
    size_t *stack_ptr = stack;

    // iterate over the source tokens, tracking loops as we go
    for (size_t i = 0; i < tokens->num_tokens; ++i)
    {
        bf_token *curr_token = (tokens->tokens)[i];
        switch (curr_token->token_type)
        {
            case L_BRACKET:
                // push the token index onto the token stack
                *(stack_ptr++) = i;
                break;

            case R_BRACKET:
                /* attempt to pop a left bracket off the stack. If the stack is empty,
                 * then we have an unmatched bracket and we return this token
                 */
                if (stack_ptr == stack)
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
    if (stack_ptr != stack)
    {
        /* one or more unmatched open brackets.
         * send back the first unmatched token
         */
        return (tokens->tokens)[stack[0]];
    }

    // no unmatched tokens found
    return NULL;
}

void interpret(bf_tokens *tokens)
{
    // track loops
    bf_token **loop_stack[tokens->num_tokens];
    bf_token ***loop_stack_ptr = loop_stack;

    /* The program array.
     * This implementation uses byte-sized cells
     * and is fixed at a size of 30000 cells
     */
    char program[30000] = { 0 };
    // the data pointer
    char *data_ptr = program;

    // the instruction pointer
    bf_token **instr_ptr = tokens->tokens;

    // interpret until we've consumed all tokens
    while ((size_t)(instr_ptr - tokens->tokens) < tokens->num_tokens)
    {
        switch ((*instr_ptr)->token_type)
        {
            // increment the data pointer
            case RIGHT_SHIFT:
                ++data_ptr;
                ++instr_ptr;
                break;

            // decrement the data pointer
            case LEFT_SHIFT:
                --data_ptr;
                ++instr_ptr;
                break;

            // increment the byte at the data pointer
            case PLUS:
                ++(*data_ptr);
                ++instr_ptr;
                break;

            // decrement the byte at the data pointer
            case MINUS:
                --(*data_ptr);
                ++instr_ptr;
                break;

            // output the byte at the data pointer
            case DOT:
                putchar(*data_ptr);
                ++instr_ptr;
                break;

            // store a byte read from stdin into the byte at the data pointer
            case COMMA:
                *data_ptr = getchar();
                ++instr_ptr;
                break;

            /* if the byte at the data pointer is nonzero, increment the
             * instruction pointer, else jump the instruction pointer
             * forward to the instruction after the matching R_BRACKET
             */
            case L_BRACKET:
                if (*data_ptr)
                {
                    /* store the current instruction position before
                     * incrementing the instruction pointer so we can
                     * jump back here later if needed
                     */
                    *(loop_stack_ptr++) = instr_ptr++;
                }
                else
                {
                    /* track unmatched L_BRACKET tokens encountered while jumping
                     * to know when we've reached the matching R_BRACKET
                     */
                    size_t unmatched_lbrackets = 0;

                    // jump to the matching R_BRACKET
                    int curr_token_type = (*(++instr_ptr))->token_type;
                    while (curr_token_type != R_BRACKET || unmatched_lbrackets)
                    {
                        // encountered another L_BRACKET that needs to be matched
                        if (curr_token_type == L_BRACKET)
                        {
                            ++unmatched_lbrackets;
                        }
                        /* encountered an R_BRACKET that matches an L_BRACKET
                         * that was encountered while jumping
                         */
                        else if (curr_token_type == R_BRACKET)
                        {
                            --unmatched_lbrackets;
                        }
                        // move to next token
                        curr_token_type = (*(++instr_ptr))->token_type;
                    }
                    // move to the instruction following the matching R_BRACKET
                    ++instr_ptr;
                }
                break;

            /* if the byte at the data pointer is zero, increment the
             * instruction pointer, else jump the instruction pointer
             * back to the instruction after the matching L_BRACKET
             */
            case R_BRACKET:
                if (!*data_ptr)
                {
                    /* loop won't continue, so pop the matching L_BRACKET
                     * off the loop stack, then move to the next instruction
                     */
                    --loop_stack_ptr;
                    ++instr_ptr;
                }
                else // jump back to instruction after matching L_BRACKET
                {
                    instr_ptr = *(loop_stack_ptr - 1) + 1;
                }
                break;
        }
    }
}
