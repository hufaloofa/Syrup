#include <stdio.h>
#include <stdlib.h>

#include "../include/lexer.h"
#include "../include/parser.h"
#include "../include/expr.h"
#include "../include/debug.h"

#define MAX_TOKENS 1024

void run(char* source) {
    Lexer lexer = initLexer(source);
    Token *ts = malloc(sizeof(Token) * MAX_TOKENS);
    scanTokens(&lexer, ts); // gives tokens

    // NOTE: FOR NOW TOKENS ARE STORED IN ts AND NOT IN LEXER

    // for (Token *t = ts; t->type != _EOF; t++) {
    //     print_token(t);
    // }

    Parser parser = initParser(ts);
    for (Token *t = parser.tokens; t->type != _EOF; t++) {
        print_token(t);
    }
    // Expr *expression = parse(source)
}