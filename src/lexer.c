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

Token makeToken(TokenType type, char* value) {
    Token token;
    token.type = type;
    token.start = lexer.start;
    token.line = lexer.line;
    token.length = (int)(lexer.current - lexer.start);
    token.value = value;
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
    int length = lexer.current - lexer.start;
    char* temp = malloc(length + 1);
    memcpy(temp, lexer.start+1, length-1);
    temp[length] = '\0';
    nextChar();
    return makeToken(_STRING, temp);   
}

Token number() {
    while (isDigit(peek())) nextChar();
    if (peek() == '.' && isDigit(peekNext())) {
        nextChar();
        while (isDigit(peek())) nextChar();
    }
    int length = lexer.current - lexer.start;
    char* temp = malloc(length + 1);
    memcpy(temp, lexer.start, length);
    temp[length] = '\0';
    return makeToken(_NUMBER, temp);

}

/*
    ie for else. if lexer.current is 14 and lexer.start is 10
    since we know the start and end of the char, then 14-10 must be 
    equal to 1 (start) + 3 (length)
 */
TokenType checkWord(int start, int length, char* c, TokenType type) {
    if (lexer.current - lexer.start == start + length && memcmp(lexer.start + start, c, length) == 0) {
        return type;    
    }

    return _IDENTIFIER;
}

TokenType identifierType() {
    switch (lexer.start[0]) {
        case 'a':
            return checkWord(1, 2, "nd", _AND);
        case 'd':
            return checkWord(1, 2, "ef", _DEF);
        case 'e':
            return checkWord(1, 3, "lse", _ELSE);
        case 'f':
            if (lexer.current - lexer.start > 1) {
                switch (lexer.start[1]) {
                    case 'a':
                        return checkWord(2, 3, "lse", _FALSE);
                    case 'o':
                        return checkWord(2, 1, "r", _FOR);
                }
            }
        case 'l':
            return checkWord(1, 2, "et", _LET);
        case 'i':
            return checkWord(1, 1, "f", _IF);
        case 'o':
            return checkWord(1, 1, "r", _OR);
        case 'p':
            return checkWord(1, 4, "rint", _PRINT);
        case 'r':
            return checkWord(1, 5, "eturn", _RETURN);
        case 't':
            return checkWord(1, 3, "rue", _TRUE);
        default:
            return _IDENTIFIER;

        
    }
}

Token identifier() {
    while (isAlpha(peek()) || isDigit(peek())) nextChar(); // skips to end of char
    int length = lexer.current - lexer.start;
    char* temp = malloc(length + 1);
    memcpy(temp, lexer.start, length);
    temp[length] = '\0';
    return makeToken(identifierType(), temp);
}

void scanTokens() {
    while (!isAtEnd()) {
        lexer.start = lexer.current;
        Token token = scanToken();
        // Token exemplar;
        // exemplar = scanToken();
        // printf("%u : %s\n", exemplar.type, exemplar.value);
    }
}

Token scanToken() {
    skipWhiteSpace();
    lexer.start = lexer.current;
    if (isAtEnd()) return makeToken(_EOF, "\0");
    char c = nextChar(); // goes to next char but returns "current char"
    bool isEqual;
    if (isAlpha(c)) {
        return identifier();
    }
    if (isDigit(c)) {
        return number();
    }

    switch (c) {
        case ';':
            return makeToken(_SEMICOLON, ";");
        case '(':
            return makeToken(_LEFT_PAR, "(");
        case ')':
            return makeToken(_RIGHT_PAR, ")");
        case '{':
            return makeToken(_LEFT_CURLY, "{");
        case '}':
            return makeToken(_RIGHT_CURLY, "}");
        case '+':
            return makeToken(_PLUS, "+");
        case '-':
            return makeToken(_MINUS, "-");
        case '=':
            isEqual = match('=');
            return makeToken(isEqual ? _EQUAL_EQUAL : _EQUAL, isEqual ? "==" : "=");
        case '"':
            return string();
        case ',':
            return makeToken(_COMMA, ",");
        case '/':
            return makeToken(_SLASH, "/");
        case '*':
            return makeToken(_STAR, "*");
        case '.':
            return makeToken(_DOT, ".");
        case '!':
            isEqual = match('=');
            return makeToken(isEqual ? _BANG_EQUAL : _BANG, isEqual ? "!=" : "!");
        case '>':
            isEqual = match('=');  
            return makeToken(isEqual ? _GREATER_THAN : _GREATER, isEqual ? ">=" : ">");
        case '<':
            isEqual = match('=');
            return makeToken(isEqual ? _LESS_THAN : _LESS, isEqual ? "<=" : "<");
        // default:

    }
    
    return errorToken("What character is this? You fool!");
}




