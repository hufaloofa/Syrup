#include "../include/debug.h"

void print_token(Token *token) {
    char *type;
    switch (token->type) {
        case _NUMBER: type = "NUMBER"; break;
        case _STRING: type = "STRING"; break;
        case _TRUE:   type = "TRUE"; break;
        case _FALSE:  type = "FALSE"; break;
        case _NIL:    type = "NIL"; break;
        case _LET:    type = "LET"; break;
        case _IF:     type = "IF"; break;
        case _ELSE:   type = "ELSE"; break;
        case _AND:    type = "AND"; break;
        case _OR:     type = "OR"; break;
        case _RIGHT_PAR: type = "RIGHT_PAR"; break;
        case _LEFT_PAR: type = "LEFT_PAR"; break;
        case _RIGHT_CURLY: type = "RIGHT_CURLY"; break;
        case _LEFT_CURLY: type = "LEFT_CURLY"; break;
        case _SEMICOLON: type = "SEMICOLON"; break;
        case _COMMA:  type = "COMMA"; break;
        case _EQUAL:  type = "EQUAL"; break;
        case _BANG:   type = "BANG"; break;
        case _PLUS:   type = "PLUS"; break;
        case _MINUS:  type = "MINUS"; break;
        case _EOF:    type = "EOF"; break;
        case _SLASH:    type = "SLASH"; break;
        case _BANG_EQUAL:  type = "BANG_EQUAL"; break;
        
    }

    printf("TOKEN[type: %s, value: \"%s\", line: %d]\n",
            type, token->value, token->line);
}