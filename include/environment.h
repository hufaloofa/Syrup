#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <stdio.h>
#include <stdlib.h>

#include "expr.h"
#include "../util/map.h"
#include "lexer.h"

typedef struct Env Env;

struct Env{
    map values;
    Env *enclosing;
};

Env *env_create(Env *enclosing);
void env_define(Env * env, char* name, Expr *value);
Expr *env_get(Env *env, Token *name);
void env_assign(Env *env, Token *name, Expr *value);

#endif