#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <stdio.h>
#include <stdlib.h>

#include "expr.h"
#include "../util/map.h"
#include "lexer.h"

typedef struct Env Env;

struct Env{
    Map *values;
    Env *enclosing;
};

Env *env_create(Env *enclosing);
void env_define(Env * env, char* name, Expr *value);
Expr *env_get(Env *env, Token *name);
void env_assign(Env *env, Token *name, Expr *value);
Expr *env_get_at(Env *env, size_t distance, char *name);
Env *ancestor(Env *env, size_t distance);
void env_assign_at(Env *env, size_t distance, Token *name, Expr *value);
#endif