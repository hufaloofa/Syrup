#include <stdio.h>
#include <stdlib.h>

typedef struct {
    char* start;
    char* current;
    int line;
} Lexer;

typedef enum {
    _SEMICOLON, _LEFT_PAR, _RIGHT_PAR, _LEFT_CURLY, _RIGHT_CURLY, _PLUS, _MINUS,
    _EQUAL, _EOF, _STRING, _COMMA, _ERROR
} TokenType;

typedef struct {
    TokenType type;
    char* start;
    int length;
    int line;
} Token;

void initLexer(char* source);
char peek();


void skipWhiteSpace();

Token scanToken();

char nextChar();