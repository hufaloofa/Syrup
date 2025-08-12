#include <stdio.h>
#include <stdlib.h>

#include "../include/lexer.h"
#include "../include/parser.h"
#include "../include/expr.h"
#include "../include/debug.h"
#include "../include/interpreter.h"
#include "../include/statement.h"
#include "../util/vector.h"
#include "../include/environment.h"
#include "../include/resolver.h"

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
    // for (Token *t = parser.tokens; t->type != _EOF; t++) {
    //     print_token(t);
    // }

    // Expr *ast = parse_expr(&parser);

    // if (ast != NULL) {
    //     print_expr(ast);
    // } else {
    //     printf("parsing failed XD");
    // }
    Vector *statements = parse_stmt(&parser);
    // if (statements == NULL || vector_size(statements) == 0) {
    //     printf("No statements parsed\n");
    //     free(ts);
    //     return;
    // }

    // printf("\n");
    // printf("\n");
    
    // interpret_expr(ast);
    resolve(statements);
    // free(ts);
    interpret_stmt(statements);
}