#include <stdlib.h>

#include "../include/statement.h"
#include "../include/expr.h"

PrintStmt *print_statement(Expr* value) {
    PrintStmt *print_statement = malloc(sizeof(PrintStmt));
    print_statement->base.type = STMT_PRINT;
    print_statement->expression = value;
    return print_statement;
}

ExprStmt *expression_statement(Expr* expr) {
    ExprStmt *expression_statement = malloc(sizeof(ExprStmt));
    expression_statement->base.type = STMT_EXPRESSION;
    expression_statement->expression = expr;
    return expression_statement;
}