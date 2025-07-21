#include <stdio.h>
#include <stdlib.h>

#include "../include/lexer.h"

void compile(char* source) {
    Token exemplar;
    Token exemplar2;
    initLexer(source);
    // printf("%s", source);
    // char current = peek();
    // printf("%c\n", current);
    // skipWhiteSpace();
    // char new = peek();
    // printf("%c\n", new);
    skipWhiteSpace();
    char current = peek();
    printf("%c\n", current);
    exemplar = scanToken();
    printf("%u\n", exemplar.type);
    
    // nextChar();
    skipWhiteSpace();
    char newcurrent = peek();
    printf("%c\n", newcurrent);
    exemplar2 = scanToken();
    printf("%u\n", exemplar2.type);

}