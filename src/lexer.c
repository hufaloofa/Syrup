#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <strings.h>

#include "../include/lexer.h"
#include "../include/debug.h"


Lexer initLexer(char* source) {
    Lexer lexer;
    lexer.start = source;
    lexer.current = source;
    lexer.line = 1;
    return lexer;
}

bool isAtEnd(Lexer *lexer) {
    return *lexer->current == '\0';
}

bool isAlpha(char c) {
    return (c >= 'a' && c<= 'z') || (c >= 'A' && c <= 'Z') || (c == '_');
}

bool isDigit(char c) {
    return (c >= '0' && c<= '9');
}

char nextChar(Lexer *lexer) {
    lexer->current++;
    return lexer->current[-1];
} 

char peek(Lexer *lexer) {
    return *lexer->current;
}

char peekNext(Lexer *lexer) {
    if (isAtEnd(lexer)) return '\0';
    return lexer->current[1];
}

void skipWhiteSpace(Lexer *lexer) {
    for (;;) {
        char c = peek(lexer);
        switch (c) {
            case ' ':
            case '\r':
            case '\t': // tab case
                nextChar(lexer);
                break;
            case '\n': // new line case
                lexer->line++;
                nextChar(lexer);
                break;
            // comment case
            case '/':
                if (peekNext(lexer) == '/') {
                    while (peek(lexer)!='\n' && !isAtEnd(lexer)) nextChar(lexer); 
                } else {
                    return;
                }
                break;
            default:
                return;
        }
    }
}

Token *makeToken(Lexer *lexer, TokenType type, char* value) {
    Token *token = lexer->tokens++;
    token->type = type;
    token->start = lexer->start;
    token->line = lexer->line;
    token->length = (int)(lexer->current - lexer->start);
    token->value = value;
    return token;
}

Token *makeExprToken(TokenType type, char* value) {
    Token *token = NULL;
    token->type = type;
    token->value = value;
    return token;
}

Token *errorToken(Lexer *lexer, char* errorMessage) {
    Token *token = lexer->tokens++;
    token->type = _ERROR;
    token->start = errorMessage;
    token->line = lexer->line;
    token->length = (int)strlen(errorMessage);
    return token;
}

bool match(Lexer *lexer, char c) {
    if (isAtEnd(lexer)) return false;
    if (*lexer->current != c) return false;

    lexer->current++;
    return true;
}

Token *string(Lexer *lexer) {
    while (peek(lexer) != '"' && !isAtEnd(lexer)) {
        if (peek(lexer) == '\n') lexer->line++;
        nextChar(lexer);
    }
    if (isAtEnd(lexer)) {
        return errorToken(lexer, "Unterminated string you fool!");
    }
    int length = lexer->current - lexer->start;
    char* temp = malloc(length + 1);
    memcpy(temp, lexer->start+1, length-1);
    temp[length] = '\0';
    nextChar(lexer);
    return makeToken(lexer, _STRING, temp);   
}

Token *number(Lexer *lexer) {
    while (isDigit(peek(lexer))) nextChar(lexer);
    if (peek(lexer) == '.' && isDigit(peekNext(lexer))) {
        nextChar(lexer);
        while (isDigit(peek(lexer))) nextChar(lexer);
    }
    int length = lexer->current - lexer->start;
    char* temp = malloc(length + 1);
    memcpy(temp, lexer->start, length);
    temp[length] = '\0';
    return makeToken(lexer, _NUMBER, temp);

}

/*
    ie for else. if lexer.current is 14 and lexer.start is 10
    since we know the start and end of the char, then 14-10 must be 
    equal to 1 (start) + 3 (length)
 */
TokenType checkWord(Lexer *lexer, int start, int length, char* c, TokenType type) {
    if (lexer->current - lexer->start == start + length && memcmp(lexer->start + start, c, length) == 0) {
        return type;    
    }

    return _IDENTIFIER;
}

TokenType identifierType(Lexer *lexer) {
    switch (lexer->start[0]) {
        case 'a':
            return checkWord(lexer, 1, 2, "nd", _AND);
        case 'd':
            return checkWord(lexer, 1, 2, "ef", _DEF);
        case 'e':
            return checkWord(lexer, 1, 3, "lse", _ELSE);
        case 'f':
            if (lexer->current - lexer->start > 1) {
                switch (lexer->start[1]) {
                    case 'a':
                        return checkWord(lexer, 2, 3, "lse", _FALSE);
                    case 'o':
                        return checkWord(lexer, 2, 1, "r", _FOR);
                }
            }
        case 'l':
            return checkWord(lexer, 1, 2, "et", _LET);
        case 'i':
            return checkWord(lexer, 1, 1, "f", _IF);
        case 'o':
            return checkWord(lexer, 1, 1, "r", _OR);
        case 'p':
            return checkWord(lexer, 1, 4, "rint", _PRINT);
        case 'r':
            return checkWord(lexer, 1, 5, "eturn", _RETURN);
        case 't':
            return checkWord(lexer, 1, 3, "rue", _TRUE);
        default:
            return _IDENTIFIER;

        
    }
}

Token *identifier(Lexer *lexer) {
    while (isAlpha(peek(lexer)) || isDigit(peek(lexer))) nextChar(lexer); // skips to end of char
    int length = lexer->current - lexer->start;
    char* temp = malloc(length + 1);
    memcpy(temp, lexer->start, length);
    temp[length] = '\0';
    return makeToken(lexer, identifierType(lexer), temp);
}

int scanTokens(Lexer *lexer, Token *ts) {
    lexer->tokens = ts;

    int count = 0;

    while (!isAtEnd(lexer)) {
        lexer->start = lexer->current;
        scanToken(lexer);
        count++;
        // Token *exemplar;
        // exemplar = scanToken(lexer);
        // printf("%u : %s\n", exemplar->type, exemplar->value);

    }

    makeToken(lexer, _EOF, "\0");
    count++;

    return count;
}

Token *scanToken(Lexer *lexer) {
    skipWhiteSpace(lexer);
    lexer->start = lexer->current;
    if (isAtEnd(lexer)) return makeToken(lexer, _EOF, "\0");
    char c = nextChar(lexer); // goes to next char but returns "current char"
    bool isEqual;
    if (isAlpha(c)) {
        return identifier(lexer);
    }
    if (isDigit(c)) {
        return number(lexer);
    }

    switch (c) {
        case ';':
            return makeToken(lexer, _SEMICOLON, ";");
        case '(':
            return makeToken(lexer, _LEFT_PAR, "(");
        case ')':
            return makeToken(lexer, _RIGHT_PAR, ")");
        case '{':
            return makeToken(lexer, _LEFT_CURLY, "{");
        case '}':
            return makeToken(lexer, _RIGHT_CURLY, "}");
        case '+':
            return makeToken(lexer, _PLUS, "+");
        case '-':
            return makeToken(lexer, _MINUS, "-");
        case '=':
            isEqual = match(lexer, '=');
            return makeToken(lexer, isEqual ? _EQUAL_EQUAL : _EQUAL, isEqual ? "==" : "=");
        case '"':
            return string(lexer);
        case ',':
            return makeToken(lexer, _COMMA, ",");
        case '/':
            return makeToken(lexer, _SLASH, "/");
        case '*':
            return makeToken(lexer, _STAR, "*");
        case '.':
            return makeToken(lexer, _DOT, ".");
        case '!':
            isEqual = match(lexer, '=');
            return makeToken(lexer, isEqual ? _BANG_EQUAL : _BANG, isEqual ? "!=" : "!");
        case '>':
            isEqual = match(lexer, '=');  
            return makeToken(lexer, isEqual ? _GREATER_THAN : _GREATER, isEqual ? ">=" : ">");
        case '<':
            isEqual = match(lexer, '=');
            return makeToken(lexer, isEqual ? _LESS_THAN : _LESS, isEqual ? "<=" : "<");
        // default:

    }
    
    return errorToken(lexer, "What character is this? You fool!");
}





