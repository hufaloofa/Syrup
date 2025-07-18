#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "lexer.h"

typedef struct {
    char* start;
    char* current;
    int line;
} Lexer;

Lexer lexer;

void initLexer(char* source) {
    lexer.start = source;
    lexer.current = source;
    lexer.line = 1;
}

bool isAtEnd() {
    return *lexer.current == "\0";
}

bool isAlpha(char c) {
    return (c >= 'a' && c<= 'z') || (c >= 'A' && c <= 'Z') || (c == '_');
}

bool isDigit(char c) {
    return (c >= '0' && c<= '9');
}

