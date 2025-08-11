#ifndef DEBUG_H
#define DEBUG_H

#include "lexer.h"
#include "expr.h"

void print_token(Token *token);

void print_expr(Expr *expr);

void print_literal(Expr *expr);

void parse_error(const Token *token, const char *format, ...);

void interpret_error(const Token *token, const char *format, ...);

#endif