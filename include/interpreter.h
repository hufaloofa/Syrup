#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "expr.h"
#include "../util/vector.h"
#include "../include/interpreter.h"
#include "../include/environment.h"

typedef struct {
    Env *environment;
    Env *global;
} Interpreter;

void *interpret_expr(Expr *expr);
void interpret_stmt(Vector* statements);
Expr *executeBlock(Vector *statements, Env *env);

#endif