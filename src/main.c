#include <stdio.h>
#include "tokens.h"

int main(void)
{
    for (int i = 0; i < sizeof(TOKEN_CHARS) / sizeof(char); ++i)
    {
        printf("%c", TOKEN_CHARS[i]);
    }
    printf("\n");
    return 0;
}
