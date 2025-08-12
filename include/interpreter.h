#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "expr.h"
#include "../util/vector.h"
#include "../include/interpreter.h"
#include "../include/environment.h"

typedef struct {
    Env *environment;
    Env *global;
    Map *locals;
} Interpreter;

void *interpret_expr(Expr *expr);
void interpret_stmt(Vector* statements);
Expr *executeBlock(Vector *statements, Env *env);

void interpreter_resolve(Expr *expr, size_t depth);
#endif