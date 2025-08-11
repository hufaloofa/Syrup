#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../include/debug.h"
#include "../include/expr.h"
#include "../include/syrFunction.h"
#include "../include/environment.h"
#include "../include/interpreter.h"

SyrFunction *make_function(FunctionStmt *declaration, Env *closure, bool isInitializer) {
    SyrFunction *function = malloc(sizeof(SyrFunction));
    function->base.type = SYR_CALLABLE_FUNCTION;
    function->declaration = declaration;
    function->closure = closure;
    function->isInitializer = isInitializer;
    return function;
}

Expr *functionCall(SyrFunction *function, Vector *arguments) {
    Env *env = env_create(function->closure);
    size_t size = vector_size(arguments);
    

    for (size_t i = 0; i < size; i++) {
        Token *param = vector_at(function->declaration->parameters, i);
        Expr *arg = vector_at(arguments, i);
        env_define(env, param->value, arg);
    }

    executeBlock(function->declaration->body, env);
    return NULL;
}

size_t functionArity(SyrFunction *function) {
    return vector_size(function->declaration->parameters);
}

char *functionToString(SyrFunction *callable) {
    static char str[256];
    sprintf(str, "<fn %s>", callable->declaration->name->value);
    return str;
}