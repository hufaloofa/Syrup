#ifndef EXPR_H
#define EXPR_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "lexer.h"

typedef struct Expr Expr;


typedef enum {
    EXPR_NONE, //??
    EXPR_NIL, 
    EXPR_BOOL, 
    EXPR_NUMBER,  
    EXPR_STRING, 
    EXPR_UNARY,
    EXPR_BINARY,
    EXPR_GROUPING
} ExprType;

struct Expr{
    ExprType type;
    union {
        struct {
            Token* token;
            union {
                bool boolean;
                double number;
                char* string;
            };
        } literal;

        struct {
            Token* op;
            Expr *rhs;
        } unary;

        struct {
            Token* op;
            Expr *lhs;
            Expr *rhs;
        } binary;

        Expr *grouping;
    };
};

Expr *make_num_expr(double num, Token *token);
Expr *make_string_expr(char *string, Token *token);
Expr *make_bool_expr(bool b, Token* token);
Expr *make_nil_expr(Token *token);
Expr *make_unary_expr(Token *op, Expr* rhs);
Expr *make_binary_expr(Token* op, Expr* lhs, Expr* rhs);
Expr *make_grouping_expr(Expr *inner);

#endif