#ifndef STATEMENT_H
#define STATEMENT_H

#include <stdlib.h>
#include "../include/expr.h"
#include "../util/vector.h"

typedef enum {
    STMT_BLOCK,
    STMT_CLASS,
    STMT_EXPRESSION,
    STMT_FUNCTION,
    STMT_IF,
    STMT_PRINT,
    STMT_RETURN,
    STMT_LET,
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

typedef struct {
    Stmt base;
    Token *name;
    Expr *initialiser;
} LetStmt;

typedef struct {
    Stmt base;
    Vector *statements;
} BlockStmt;

PrintStmt *make_print_statement(Expr* value);
ExprStmt *make_expression_statement(Expr* expr);
LetStmt *make_let_statement(Token *name, Expr *initialiser);
BlockStmt *make_block_statement(Vector* statements);
#endif