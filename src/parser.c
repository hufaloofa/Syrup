#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "../include/parser.h"
#include "../include/expr.h"
#include "../include/debug.h"


Token *parser_advance(Parser *p) {
    Token *token = p->current++;
    p->eof = (p->current->type == _EOF);
    return token;
}

bool parser_check(Parser *p, TokenType type) {
    return !p->eof && p->current->type == type;
}

Token *consume(Parser *p, TokenType type) {
    if (parser_check(p, type)) {
        return parser_advance(p);
    } else {
        printf("Unexpected");
        static Token error = {_ERROR, NULL, 0, 0};
        return &error;
    }
}

bool parser_match(Parser *p, int count, ...) {
    va_list token_types;
    va_start(token_types, count);
    while (count--) {
        if (parser_check(p, va_arg(token_types, TokenType))) {
            parser_advance(p);
            return true;
        }
    }
    va_end(token_types);
    return false;
}

// a lot of overlap so defined first

Expr *expression(Parser *p);
Expr *comparison(Parser *p);
Expr *equality(Parser *p);
Expr *addition(Parser *p);
Expr *multiplication(Parser *p);
Expr *unary(Parser *p);
Expr *binary(Parser *p);
Expr *primary(Parser *p);

Expr *expression(Parser *p) {
    return equality(p);
}

Expr *equality(Parser *p) {
    Expr *expr = comparison(p);
    while (parser_match(p, 2, _EQUAL_EQUAL, _BANG_EQUAL)) {
        Token *op  = p->current-1;
        Expr *rhs = comparison(p);
        expr = make_binary_expr(op, expr, rhs);
    }
    return expr;
}

Expr *comparison(Parser *p)
{
    Expr *e = addition(p);
    while (parser_match(p, 4, _GREATER, _GREATER_THAN, _LESS, _LESS_THAN)) {
        Token *op = p->current-1;
        Expr *rhs = addition(p);
        e = make_binary_expr(op, e, rhs);
    }
    return e;
}

Expr *unary(Parser *p) {
    if (parser_match(p, 2, _BANG, _MINUS)) {
        Token* op = p->current-1;
        Expr *rhs = unary(p);
        return make_unary_expr(op, rhs);
    }

    return primary(p);
}

Expr *primary(Parser *p) {
    Token *t = p->current;
    double number;
    
    if (parser_match(p, 1, _NIL))    return make_nil_expr(p->current-1);
    if (parser_match(p, 1, _FALSE))  return make_bool_expr(false, p->current-1);
    if (parser_match(p, 1, _TRUE))   return make_bool_expr(true, p->current-1);
    if (parser_match(p, 1, _NUMBER)) {
        sscanf(t->value, "%lf", &number);
        return make_num_expr(number, p->current-1);
    }
    if (parser_match(p, 1, _STRING)) return make_string_expr(t->value, p->current-1);
    if (parser_match(p, 1, _LEFT_PAR)) {
        Expr *e = expression(p);
        consume(p, _RIGHT_PAR);
        return make_grouping_expr(e);
    }

    printf("unexpected token ");
    print_token(t);
    return &NoneExpr;

}

Expr *multiplication(Parser *p) {
    Expr *e = unary(p);
    while (parser_match(p, 2, _SLASH, _STAR)) {
        Token *op = p->current-1;
        Expr *rhs = unary(p);
        e = make_binary_expr(op, e, rhs);
    }
    return e;
}

Expr *addition(Parser *p) {
    Expr *e = multiplication(p);
    while (parser_match(p, 2, _PLUS, _MINUS)) {
        Token *op = p->current-1;
        Expr *rhs = multiplication(p);
        e = make_binary_expr(op, e, rhs);
    }
    return e;
}

void synchronise(Parser *p) {
    parser_advance(p);

    while (!p->eof) {
        if (p->current[-1].type == _SEMICOLON) return;
        switch (p->current->type) {
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
        parser_advance(p);
    }
}

Expr *parse(Parser *p, Token *tokens) {
    if (tokens->type == _NONE) {
        return NULL;
    }

    p->tokens = tokens;
    p->current = p->tokens;
    p->eof = false;

    Expr *e = expression(p);
    if (e->type == _NONE) {
        return NULL;
    }
    return e;
}


