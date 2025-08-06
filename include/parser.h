#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "lexer.h"
#include "expr.h"
#include "../util/vector.h"


typedef struct {
    Token *tokens;
    Token *current;
    bool eof;
} Parser;

Parser initParser(Token *ts);

Expr *parse_expr(Parser *parser);
Vector *parse_stmt(Parser *parser);
#endif