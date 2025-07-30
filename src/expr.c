#include <stdio.h>
#include <stdlib.h>

#include "../include/expr.h"

Expr *make_expr(ExprType t) {
    Expr *expr = malloc(sizeof(Expr));
    expr->type = t;
    return expr;
}

Expr *make_literal_expr(ExprType et, Token *token) {
    Expr *expr = make_expr(et);
    expr->literal.token = token;
    return expr;
}

Expr *make_num_expr(double num, Token *token) {
    Expr *expr = make_literal_expr(EXPR_NUMBER, token);
    expr->literal.number = num;
    return expr;
}

Expr *make_string_expr(char *string, Token *token) {
    Expr *expr = make_literal_expr(EXPR_STRING, token);
    expr->literal.string = string;
    return expr;
}

Expr *make_bool_expr(bool b, Token* token) {
    Expr *expr = make_literal_expr(EXPR_BOOL, token);
    expr->literal.boolean = b;
    return expr;
}

Expr *make_nil_expr(Token *token) {
    return make_literal_expr(EXPR_NIL, token);
}

Expr *make_unary_expr(Token* op, Expr* rhs) {
    Expr *expr = make_expr(EXPR_UNARY);
    expr->unary.op = op;
    expr->unary.rhs = rhs;
    return expr;
}

Expr *make_binary_expr(Token* op, Expr* lhs, Expr* rhs) {
    Expr *expr = make_expr(EXPR_BINARY);
    expr->binary.op = op;
    expr->binary.lhs = lhs;
    expr->binary.rhs = rhs;
    return expr;
}

Expr *make_grouping_expr(Expr *inner) {
    Expr *expr = make_expr(EXPR_GROUPING);
    expr->grouping = inner;
    return expr;
}

Expr *make_none_expr() {
    Expr *expr = make_expr(EXPR_NONE);
    return expr;
}