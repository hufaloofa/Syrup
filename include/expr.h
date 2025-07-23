#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"

typedef enum {
    EXPR_NONE,
    EXPR_BOOL,
    EXPR_NUMBER,
    EXPR_STRING,
    EXPR_UNARY,
    EXPR_BINARY,
    EXPR_GROUPING
} ExprType;

typedef struct {
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
            Expr rhs;
        } unary;

        struct {
            Token* op;
            Expr lhs;
            Expr rhs;
        } binary;

        Expr *grouping;
    };
} Expr;