#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../include/lexer.h"

Lexer lexer;

void initLexer(char* source) {
    lexer.start = source;
    lexer.current = source;
    lexer.line = 1;
}

bool isAtEnd() {
    return *lexer.current == '\0';
}

bool isAlpha(char c) {
    return (c >= 'a' && c<= 'z') || (c >= 'A' && c <= 'Z') || (c == '_');
}

bool isDigit(char c) {
    return (c >= '0' && c<= '9');
}

char nextChar() {
    lexer.current++;
    return lexer.current[-1];
} 

char peek() {
    return *lexer.current;
}

void skipWhiteSpace() {
    for (;;) {
        char c = peek();
        switch (c) {
            case ' ':
            case '\r':
            case '\t':
                nextChar();
                break;
            case '\n':
                lexer.line++;
                nextChar();
                break;
            default:
                return;
        }
    }
}

Token makeToken(TokenType type) {
    Token token;
    token.type = type;
    token.start = lexer.start;
    token.line = lexer.line;
    token.length = (int)(lexer.current - lexer.start);
    return token;
}

Token scanToken() {
    skipWhiteSpace();
    lexer.start = lexer.current;
    if (isAtEnd()) return makeToken(_EOF);
    char c = nextChar();
    // if (isAlpha(c))
    // if (isDigit(c))

    switch (c) {
        case ';':
            return makeToken(_SEMICOLON);
        case '(':
            return makeToken(_LEFT_PAR);
        case ')':
            return makeToken(_RIGHT_PAR);
        case '{':
            return makeToken(_LEFT_CURLY);
        case '}':
            return makeToken(_RIGHT_CURLY);
        case '+':
            return makeToken(_PLUS);
        case '-':
            return makeToken(_MINUS);
        case '=':
            return makeToken(_EQUAL);
        case '"':
            return makeToken(_STRING);
        case ',':
            return makeToken(_COMMA);
    }
    
    // placeholder
    return makeToken(_ERROR);
}


