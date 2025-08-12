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

PrintLnStmt *make_println_statement(Expr* value) {
    PrintLnStmt *println_statement = malloc(sizeof(PrintLnStmt));
    println_statement->base.type = STMT_PRINTLN;
    println_statement->expression = value;
    return println_statement;
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
IfStmt *make_if_statement(Expr *condition, Stmt *thenBranch, Stmt *elseBranch) {
    IfStmt *if_statement = malloc(sizeof(IfStmt));
    if_statement->base.type = STMT_IF;
    if_statement->condition = condition;
    if_statement->elseBranch = elseBranch;
    if_statement->thenBranch = thenBranch;
    return if_statement;
}

WhileStmt *make_while_statement(Expr *condition, Stmt *body) {
    WhileStmt *while_statement = malloc(sizeof(WhileStmt));
    while_statement->base.type = STMT_WHILE;
    while_statement->condition = condition;
    while_statement->body = body;
    return while_statement;
}

FunctionStmt *make_function_statement(Token *name, Vector *parameters, Vector *body) {
    FunctionStmt *function_statement = malloc(sizeof(FunctionStmt));
    function_statement->base.type = STMT_FUNCTION;
    function_statement->name = name;
    function_statement->parameters = parameters;
    function_statement->body = body;
    return function_statement;
}

ReturnStmt *make_return_statement(Token *keyword, Expr *value) {
    ReturnStmt *return_statement = malloc(sizeof(ReturnStmt));
    return_statement->base.type = STMT_RETURN;
    return_statement->keyword = keyword;
    return_statement->value = value;
    return return_statement;
}


