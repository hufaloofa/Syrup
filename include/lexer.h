#include <stdio.h>
#include <stdlib.h>

typedef struct {
    char* start;
    char* current;
    int line;
} Lexer;

typedef enum {
    // single char tokens   
    _SEMICOLON, _LEFT_PAR, _RIGHT_PAR, _LEFT_CURLY, _RIGHT_CURLY, 
    _SLASH, _PLUS, _MINUS, _COMMA, _STAR, 
    _DOT, // 10

    // one or two char // 11
    _BANG, _BANG_EQUAL, _EQUAL, _EQUAL_EQUAL,  _GREATER, 
    _GREATER_THAN, _LESS, _LESS_THAN, 

    // literals, identifier = var name // 19
    _STRING, _NUMBER, _IDENTIFIER,

    // key words // 22
    _AND, _FOR, _OR, _ELSE, _IF, 
    _TRUE, _FALSE, _PRINT, _RETURN, _WHILE, 
    _LET, _DEF, _ERROR, 
    
    // 35
    _EOF, 

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

void scanTokens();