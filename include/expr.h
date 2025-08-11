#ifndef EXPR_H
#define EXPR_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "lexer.h"
#include "../util/vector.h"
#include "syrCallable.h"
    
typedef struct Expr Expr;


typedef enum {
    EXPR_NONE,
    EXPR_NIL, 
    EXPR_BOOL, 
    EXPR_NUMBER,  
    EXPR_STRING, 
    EXPR_UNARY,
    EXPR_BINARY,
    EXPR_GROUPING,
    EXPR_LET,
    EXPR_ASSIGN,
    EXPR_LOGICAL,
    EXPR_POSTFIX,
    EXPR_CALL,
    EXPR_SYR_CALLABLE,
    EXPR_SYR_FUNCTION,
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

        struct {
            Token* op;
            Expr *lhs;
            Expr *rhs;
        } logical;

        Expr *grouping;

        struct {
            Expr *lhs;
            Token *op;
        } postfix;

        struct {
            Expr *callee;
            Token *paren;
            Vector *arguments;
        } call;

        struct {
            SyrCallable *callable;
        } SyrCallable;
    };

    //assignment only
    Expr *value;
};

static Expr NoneExpr  = { .type = EXPR_NONE };

Expr *make_num_expr(double num, Token *token);
Expr *make_string_expr(char *string, Token *token);
Expr *make_bool_expr(bool b, Token* token);
Expr *make_nil_expr(Token *token);
Expr *make_unary_expr(Token *op, Expr* rhs);
Expr *make_binary_expr(Token* op, Expr* lhs, Expr* rhs);
Expr *make_grouping_expr(Expr *inner);
Expr *make_none_expr();
Expr *make_postfix_expr(Token *op, Expr* lhs);

Expr *make_let_expr(char *string, Token *token);
Expr *make_assign_expr(Token *name, Expr *value);
Expr *make_logical_expr(Expr *left, Token *op, Expr *right);
Expr *make_call_expr(Expr *callee, Token *paren, Vector *arguments);
Expr *make_syr_callable_expr(SyrCallable *callable);
// // only used when evaluating ast
// Expr *make_num_expr_eval(double num);
// Expr *make_bool_expr_eval(bool b);
// Expr *make_string_expr_eval(char *string);

#endif