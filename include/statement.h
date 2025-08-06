#ifndef STATEMENT_H
#define STATEMENT_H

#include <stdlib.h>
#include "../include/expr.h"

typedef enum {
    STMT_BLOCK,
    STMT_CLASS,
    STMT_EXPRESSION,
    STMT_FUNCTION,
    STMT_IF,
    STMT_PRINT,
    STMT_RETURN,
    STMT_VAR,
    STMT_WHILE,
} StmtType;

typedef struct {
    StmtType type;
} Stmt;

typedef struct {
    Stmt base;
    Expr *expression;
} PrintStmt;

typedef struct {
    Stmt base;
    Expr *expression;
} ExprStmt;

PrintStmt *print_statement(Expr* value);
ExprStmt *expression_statement(Expr* expr);
#endif