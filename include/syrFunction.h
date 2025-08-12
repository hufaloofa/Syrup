#ifndef SYRFUNCTION_H
#define SYRFUNCTION_H

#include <stdbool.h>
#include "syrCallable.h"
#include "../util/vector.h"
#include "statement.h"
#include "environment.h"

typedef struct {
    SyrCallable base;
    FunctionStmt *declaration;
    Env *closure;
    bool isInitializer; 
} SyrFunction;

SyrFunction *make_function(FunctionStmt *declaration, Env *closure);
Expr *functionCall(SyrFunction *function, Vector *arguments);
size_t functionArity(SyrFunction *function);
char *functionToString(SyrFunction *callable);
#endif