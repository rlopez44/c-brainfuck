#ifndef TOKENS_H_
#define TOKENS_H_

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

#endif
