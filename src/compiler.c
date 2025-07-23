#include <stdio.h>
#include <stdlib.h>

#include "../include/lexer.h"

void compile(char* source) {
    initLexer(source);
    scanTokens();
}