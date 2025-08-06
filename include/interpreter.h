#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "expr.h"
#include "../util/vector.h"

void *interpret_expr(Expr *expr);
void interpret_stmt(Vector* statements);



#endif