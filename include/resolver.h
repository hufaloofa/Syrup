#ifndef RESOLVER_H
#define RESOLVER_H
#include "interpreter.h"
#include "statement.h"
#include "../util/vector.h"

typedef enum {
    FUNCTION_NONE,
    FUNCTION_FUNCTION,
} FunctionType;


void resolveStatements(Vector *statements);
void resolveBlockStatement(BlockStmt *stmt);
void resolveExpression(Expr *expr);
void resolvePrintStatement(PrintStmt *stmt);
void resolveLetStatement(LetStmt *stmt);
void resolveIfStatement(IfStmt *stmt);

void resolveWhileStatement(WhileStmt *stmt);
void resolveFunctionStatement(FunctionStmt *stmt);
void resolveReturnStatement(ReturnStmt *stmt);
void resolve(Vector *statements);
void resolveStatement(Stmt *stmt);
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

#endif