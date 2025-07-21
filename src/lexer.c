#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <strings.h>

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

char peekNext() {
    if (isAtEnd()) return '\0';
    return lexer.current[1];
}

void skipWhiteSpace() {
    for (;;) {
        char c = peek();
        switch (c) {
            case ' ':
            case '\r':
            case '\t': // tab case
                nextChar();
                break;
            case '\n': // new line case
                lexer.line++;
                nextChar();
                break;
            // comment case
            case '/':
                if (peekNext() == '/') {
                    while (peek()!='\n' && !isAtEnd()) nextChar(); 
                } else {
                    return;
                }
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

Token errorToken(char* errorMessage) {
    Token token;
    token.type = _ERROR;
    token.start = errorMessage;
    token.line = lexer.line;
    token.length = (int)strlen(errorMessage);
    return token;
}

bool match(char c) {
    if (isAtEnd()) return false;
    if (*lexer.current != c) return false;

    lexer.current++;
    return true;
}

Token string() {
    while (peek() != '"' && !isAtEnd()) {
        if (peek() == '\n') lexer.line++;
        nextChar();
    }
    if (isAtEnd()) {
        return errorToken("Unterminated string you fool!");
    }
    nextChar();
    return makeToken(_STRING);   
}

Token scanToken() {
    skipWhiteSpace();
    lexer.start = lexer.current;
    if (isAtEnd()) return makeToken(_EOF);
    char c = nextChar(); // goes to next char but returns "current char"
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
            return string();
        case ',':
            return makeToken(_COMMA);
        case '/':
            return makeToken(_SLASH);

    }
    
    // placeholder
    return errorToken("What character is this? You fool!");
}




