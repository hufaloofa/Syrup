#ifndef STATEMENT_H
#define STATEMENT_H

#include <stdlib.h>

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

#endif