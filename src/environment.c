#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/environment.h"
#include "../util/map.h"

Env *env_create(Env *parent) {
    Env *env = malloc(sizeof(Env));
    env->values = map_create();
    env->parent = parent;
    return env;
}

void env_define(Env * env, char* name, Expr *value) {
    map_set(env->values, name, value);
}

Expr *env_get(Env *env, Token *name) {
    if (map_contains(env->values, name->value)) {
        return map_get(env->values, name->value);
    }
    fprintf(stderr, "[line %d] Runtime error: %s %s\n", name->line, "Undefined variable", name->value);
    exit(EXIT_FAILURE);
}

void env_assign(Env *env, Token *name, Expr *value) {
    if (map_contains(env->values, name->value)) {
        map_set(env->values, name->value, value);
    } else {
        fprintf(stderr, "[line %d] Runtime error: %s %s\n", name->line, "Undefined variable", name->value);
        exit(EXIT_FAILURE);
    }
}



