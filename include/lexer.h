#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include <stdlib.h>


typedef enum {
    _NONE,

    // single char tokens   
    _SEMICOLON, _LEFT_PAR, _RIGHT_PAR, _LEFT_CURLY, _RIGHT_CURLY, 
    _SLASH, _PLUS, _MINUS, _COMMA, _STAR, 
    _DOT,

    // one or two char 
    _BANG, _BANG_EQUAL, _EQUAL, _EQUAL_EQUAL,  _GREATER, 
    _GREATER_EQUAL, _LESS, _LESS_EQUAL, 

    // literals, identifier = var name
    _STRING, _NUMBER, _IDENTIFIER,

    // key words
    _AND, _FOR, _OR, _ELSE, _IF, 
    _TRUE, _FALSE, _PRINT, _RETURN, _WHILE, 
    _LET, _DEF, _ERROR, _NIL,
    
    _EOF,

} TokenType;

typedef struct {
    TokenType type;
    char* start;
    int length;
    int line;
    char* value;
} Token;

typedef struct {
    Token *tokens;
    int count;
    int capacity;
} TokenArray;

typedef struct {
    char* start;
    char* current;
    int line;

    Token *tokens;
} Lexer;

Lexer initLexer(char* source);
// char peek(Lexer *lexer);


// void skipWhiteSpace(Lexer *lexer);

Token *scanToken(Lexer *lexer);

// char nextChar(Lexer *lexer);

int scanTokens(Lexer *lexer, Token *ts);

Token *makeToken(Lexer *lexer, TokenType type, char* value);

Token *makeExprToken(TokenType type, char* value);

#endif