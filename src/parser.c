#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "../include/parser.h"
#include "../include/expr.h"
#include "../include/debug.h"

Parser parser;

void initParser(Token *ts) {
    parser.tokens = ts;
    parser.current = parser.tokens;
    parser.eof = false;
}

Token *parser_advance() {
    Token *token = parser.current++;
    parser.eof = (parser.current->type == _EOF);
    return token;
}

bool parser_check(TokenType type) {
    return !parser.eof && parser.current->type == type;
}

Token *consume(TokenType type) {
    if (parser_check(type)) {
        return parser_advance();
    } else {
        printf("Unexpected");
        static Token error = {_ERROR, NULL, 0, 0};
        return &error;
    }
}

bool parser_match(int count, ...) {
    va_list token_types;
    va_start(token_types, count);
    while (count--) {
        if (parser_check(va_arg(token_types, TokenType))) {
            parser_advance();
            return true;
        }
    }
    va_end(token_types);
    return false;
}

// a lot of overlap so defined first

Expr *expression();
Expr *comparison();
Expr *equality();
Expr *addition();
Expr *multiplication();
Expr *unary();
Expr *binary();
Expr *primary();

Expr *expression() {
    return equality();
}

Expr *equality() {
    Expr *expr = comparison();
    while (parser_match(2, _EQUAL_EQUAL, _BANG_EQUAL)) {
        Token *op  = parser.current-1;
        Expr *rhs = comparison();
        expr = make_binary_expr(op, expr, rhs);
    }
    return expr;
}

Expr *comparison()
{
    Expr *e = addition();
    while (parser_match(4, _GREATER, _GREATER_THAN, _LESS, _LESS_THAN)) {
        Token *op = parser.current-1;
        Expr *rhs = addition();
        e = make_binary_expr(op, e, rhs);
    }
    return e;
}

Expr *unary() {
    if (parser_match(2, _BANG, _MINUS)) {
        Token* op = parser.current-1;
        Expr *rhs = unary();
        return make_unary_expr(op, rhs);
    }

    return primary();
}

Expr *primary() {
    Token *t = parser.current;
    double number;
    
    if (parser_match(1, _NIL))    return make_nil_expr(parser.current-1);
    if (parser_match(1, _FALSE))  return make_bool_expr(false, parser.current-1);
    if (parser_match(1, _TRUE))   return make_bool_expr(true, parser.current-1);
    if (parser_match(1, _NUMBER)) {
        sscanf(t->value, "%lf", &number);
        return make_num_expr(number, parser.current-1);
    }
    if (parser_match(1, _STRING)) return make_string_expr(t->value, parser.current-1);
    if (parser_match(1, _LEFT_PAR)) {
        Expr *e = expression();
        consume(_RIGHT_PAR);
        return make_grouping_expr(e);
    }

    printf("unexpected token ");
    print_token(t);
    return &NoneExpr;

}

Expr *multiplication() {
    Expr *e = unary();
    while (parser_match(2, _SLASH, _STAR)) {
        Token *op = parser.current-1;
        Expr *rhs = unary();
        e = make_binary_expr(op, e, rhs);
    }
    return e;
}

Expr *addition() {
    Expr *e = multiplication();
    while (parser_match(2, _PLUS, _MINUS)) {
        Token *op = parser.current-1;
        Expr *rhs = multiplication();
        e = make_binary_expr(op, e, rhs);
    }
    return e;
}

void synchronise() {
    parser_advance();

    while (!parser.eof) {
        if (parser.current[-1].type == _SEMICOLON) return;
        switch (parser.current->type) {
            case _LET:
            case _DEF:
            case _PRINT:
            case _WHILE:
            case _FOR:
            case _IF:
            case _RETURN:
                return;
            default:
                break;
        }
        parser_advance();
    }
}

Expr *parse() {
    if (parser.tokens->type == _NONE) {
        return NULL;
    }

    Expr *e = expression();
    return e;
}


