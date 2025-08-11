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

typedef struct {
    Stmt base;
    Expr *condition;
    Stmt *thenBranch;
    Stmt *elseBranch;
} IfStmt;

typedef struct {
    Stmt base;
    Expr *condition;
    Stmt *body;
} WhileStmt;

typedef struct {
    Stmt base;
    Token *name;
    Vector *parameters;
    Vector *body;
} FunctionStmt;

typedef struct {
    Stmt base;
    Token *keyword;
    Expr *value;
} ReturnStmt;

PrintStmt *make_print_statement(Expr* value);
ExprStmt *make_expression_statement(Expr* expr);
LetStmt *make_let_statement(Token *name, Expr *initialiser);
BlockStmt *make_block_statement(Vector* statements);
IfStmt *make_if_statement(Expr *condition, Stmt *thenBranch, Stmt *elseBranch);
WhileStmt *make_while_statement(Expr *condition, Stmt *body);
FunctionStmt *make_function_statement(Token *name, Vector *parameters, Vector *body);
ReturnStmt *make_return_statement(Token *keyword, Expr *value);

#endif