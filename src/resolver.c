#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../include/resolver.h"
#include "../include/interpreter.h"
#include "../include/expr.h"
#include "../include/statement.h"
#include "../util/vector.h"
#include "../util/map.h"

void resolveStatement(Stmt *stmt);
void resolveStatements(Vector *statements);
void resolveBlockStatement(BlockStmt *stmt);
void resolveExpression(Expr *expr);
void resolvePrintStatement(PrintStmt *stmt);
void resolveLetStatement(LetStmt *stmt);
void resolveIfStatement(IfStmt *stmt);
void resolveWhileStatement(WhileStmt *stmt);
void resolveFunctionStatement(FunctionStmt *stmt);
void resolveReturnStatement(ReturnStmt *stmt);

void resolveStatement(Stmt *stmt);
void resolveExpression(Expr *expr);  
void initResolver();
void beginScope();
void endScope();
void declare(Token *name);
void define(Token *name);
void resolveCallExpr(Expr *expr);
void resolvePostfixExpr(Expr *expr);
void resolveUnaryExpr(Expr *expr);
void resolveBinaryExpr(Expr *expr);
void resolveGroupingExpr(Expr *expr);
void resolveLetExpr(Expr *expr);
void resolveAssignExpr(Expr *expr);
void resolveLogicalExpr(Expr *expr);
void resolveFunction(FunctionStmt *stmt, FunctionType type);
void resolveLocal(Expr *expr, Token *name);

struct {
    Vector *scopes; 
    FunctionType currentFunction;
} resolver;

void resolve(Vector *statements) {
    initResolver();
    resolveStatements(statements);
}


void initResolver() {
    static bool initialized = false;
    if (initialized) {
            return;
    }
    resolver.scopes = vector_construct();
    initialized = true;
    resolver.currentFunction = FUNCTION_NONE;
}

void beginScope() {
    vector_push_back(resolver.scopes, map_construct(str_compare));
}

void endScope() {
    vector_pop_back(resolver.scopes);
}

void resolveBlockStatement(BlockStmt *stmt) {
    beginScope();
    resolveStatements(stmt->statements);
    endScope();
}

void resolveFunctionStatement(FunctionStmt *stmt) {
    declare(stmt->name);
    define(stmt->name);

    resolveFunction(stmt, FUNCTION_FUNCTION);
}

void resolveIfStatement(IfStmt *stmt) {
    resolveExpression(stmt->condition);
    resolveStatement(stmt->thenBranch);
    if (stmt->elseBranch != NULL) {
        resolveStatement(stmt->elseBranch);
    }
}

void resolvePrintStatement(PrintStmt *stmt) {
    resolveExpression(stmt->expression);
}

void resolveReturnStatement(ReturnStmt *stmt) {
    if (resolver.currentFunction == FUNCTION_NONE) {
        fprintf(stderr, "Cannot return from top-level code.\n");
        exit(EXIT_FAILURE);
    }
    if (stmt->value != NULL) {
        resolveExpression(stmt->value);
    }
}

void resolveWhileStatement(WhileStmt *stmt) {
    resolveExpression(stmt->condition);
    resolveStatement(stmt->body);
}

void resolveFunction(FunctionStmt *stmt, FunctionType type) {
    FunctionType enclosingFunction = resolver.currentFunction;
    resolver.currentFunction = type;

    beginScope();
    for (size_t i = 0; i < vector_size(stmt->parameters); i++) {
        Token *param = vector_at(stmt->parameters, i);
        declare(param);
        define(param);
    }
    resolveStatements(stmt->body);
    endScope();

    resolver.currentFunction = enclosingFunction;
}

void resolveStatements(Vector *statements) {
    size_t num = vector_size(statements);
    for (size_t i = 0; i < num; i++) {
        resolveStatement(vector_at(statements, i));
    }
}

void resolveLetStatement(LetStmt *stmt) {
    declare(stmt->name);
    if (stmt->initialiser != NULL) {
        resolveExpression(stmt->initialiser);
    }
    define(stmt->name);
}


void resolveStatement(Stmt *stmt) {
    switch (stmt->type) {
        case STMT_BLOCK:
            resolveBlockStatement((BlockStmt *)stmt);
            break;
        case STMT_EXPRESSION:
            resolveExpression(((ExprStmt *)stmt)->expression);
            break;
        case STMT_PRINT:
            resolvePrintStatement(((PrintStmt *)stmt));
            break;
        case STMT_LET:
            resolveLetStatement((LetStmt *)stmt);
            break;
        case STMT_IF:
            resolveIfStatement((IfStmt *)stmt);
            break;
        case STMT_WHILE:
            resolveWhileStatement((WhileStmt *)stmt);
            break;
        case STMT_FUNCTION:
            resolveFunctionStatement((FunctionStmt *)stmt);
            break;
        case STMT_RETURN:
            resolveReturnStatement((ReturnStmt *)stmt);
            break;
        default:
            fprintf(stderr, "Unknown statement type: %d\n", stmt->type);
            exit(EXIT_FAILURE);
    }
}

void resolveExpression(Expr *expr) {
    switch (expr->type) {
        case EXPR_BINARY:
            resolveBinaryExpr(expr);
            break;
        case EXPR_UNARY:
            resolveUnaryExpr(expr);
            break;
        case EXPR_GROUPING:
            resolveGroupingExpr(expr);
            break;
        case EXPR_LET:
            resolveLetExpr(expr);
            break;
        case EXPR_ASSIGN:
            resolveAssignExpr(expr);
            break;
        case EXPR_LOGICAL:
            resolveLogicalExpr(expr);
            break;
        case EXPR_POSTFIX:
            resolvePostfixExpr(expr);
            break;
        
        case EXPR_CALL:
            resolveCallExpr(expr);
            break;
        default:
            // Handle other expression types if necessary
            break;
    }
}

void resolveLetExpr(Expr *expr) {
    if (!vector_is_empty(resolver.scopes)) {
        Map *scope = vector_at_back(resolver.scopes);
        if (map_contains(scope, expr->literal.token) && !(bool)map_get(scope, expr->literal.token)) {
            fprintf(stderr, "Variable '%s' already declared in this scope.\n",
                    expr->literal.token->value);
            exit(EXIT_FAILURE);
        }
    }
    resolveLocal(expr, expr->literal.token);
}


void resolveAssignExpr(Expr *expr) {
    resolveExpression(expr->value);
    resolveLocal(expr, expr->literal.token);
}

void resolveBinaryExpr(Expr *expr) {
    resolveExpression(expr->binary.lhs);
    resolveExpression(expr->binary.rhs);
}

void resolveCallExpr(Expr *expr) {
    resolveExpression(expr->call.callee);
    size_t num_args = vector_size(expr->call.arguments);
    for (size_t i = 0; i < num_args; i++) {
        resolveExpression(vector_at(expr->call.arguments, i));
    }
}

void resolveGroupingExpr(Expr *expr) {
    resolveExpression(expr->grouping);
}

void resolveUnaryExpr(Expr *expr) {
    resolveExpression(expr->unary.rhs);
}

void resolvePostfixExpr(Expr *expr) {
    resolveExpression(expr->postfix.lhs);
}

void resolveLogicalExpr(Expr *expr) {
    resolveExpression(expr->logical.lhs);
    resolveExpression(expr->logical.rhs);
}


void resolveLocal(Expr *expr, Token *name) {
    size_t n = vector_size(resolver.scopes);
    for (size_t i = n; i-- > 0; ) {
        Map *scope = vector_at(resolver.scopes, i);
        if (map_contains(scope, name->value)) {
            interpreter_resolve(expr, n - 1 - i);
            return;
        }
    }
}


void declare(Token *name) {
    if (vector_is_empty(resolver.scopes)) return;
    Map *scope = vector_at_back(resolver.scopes);
    if (map_contains(scope, name->value)) {
        fprintf(stderr, "Variable '%s' already declared in this scope.\n", name->value);
        exit(EXIT_FAILURE);
    }
    map_put(scope, name->value, (void *)false);

}

void define(Token *name) {
    if (vector_is_empty(resolver.scopes)) return;

    Map *scope = vector_at_back(resolver.scopes);
    map_put(scope, name->value, (void *)true);
}

