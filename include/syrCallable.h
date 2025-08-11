#ifndef SYRCALLABLE_H
#define SYRCALLABLE_H

#include "../util/vector.h"


struct Expr;
typedef struct Expr Expr;

typedef enum {
    SYR_CALLABLE_FUNCTION,
    SYR_CALLABLE_CLOCK,

} SyrCallableType;

typedef struct {
    SyrCallableType type;
} SyrCallable;

Expr *syrCallableCall(SyrCallable *callable, Vector *arguments);
size_t syrCallableArity(SyrCallable *callable);
char *syrCallableToString(SyrCallable *callable);
#endif