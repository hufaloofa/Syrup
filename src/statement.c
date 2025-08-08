#include <stdlib.h>

#include "../include/statement.h"
#include "../include/expr.h"
#include "../util/vector.h"

PrintStmt *make_print_statement(Expr* value) {
    PrintStmt *print_statement = malloc(sizeof(PrintStmt));
    print_statement->base.type = STMT_PRINT;
    print_statement->expression = value;
    return print_statement;
}

ExprStmt *make_expression_statement(Expr* expr) {
    ExprStmt *expression_statement = malloc(sizeof(ExprStmt));
    expression_statement->base.type = STMT_EXPRESSION;
    expression_statement->expression = expr;
    return expression_statement;
}

LetStmt *make_let_statement(Token *name, Expr *initialiser) {
    LetStmt *let_statement = malloc(sizeof(LetStmt));
    let_statement->base.type = STMT_LET;
    let_statement->name = name;
    let_statement->initialiser = initialiser;
    return let_statement;
}

BlockStmt *make_block_statement(Vector* statements) {
    BlockStmt *block_statement = malloc(sizeof(BlockStmt));
    block_statement->base.type = STMT_BLOCK;
    block_statement->statements = statements;
    return block_statement;
}
