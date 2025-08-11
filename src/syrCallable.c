#include <stdlib.h>
#include <time.h>
#include "../include/syrCallable.h"
#include "../include/expr.h"
#include "../util/vector.h"
#include "../include/interpreter.h";
#include "../include/syrFunction.h"



size_t syrCallableArity(SyrCallable *callable) {
    switch (callable->type) {
        case SYR_CALLABLE_FUNCTION:
            return functionArity((SyrFunction *)callable);
        case SYR_CALLABLE_CLOCK:
            return 0;
    }   
}


Expr *syrCallableCall(SyrCallable *callable, Vector *arguments) {
    switch (callable->type) {
        case SYR_CALLABLE_FUNCTION:
            return functionCall((SyrFunction *)callable, arguments);
        case SYR_CALLABLE_CLOCK:
            return make_num_expr(time(NULL), NULL); 
    }
}

char *syrCallableToString(SyrCallable *callable) {
    switch (callable->type) {
    case SYR_CALLABLE_CLOCK:
            return "<native fn>";
    case SYR_CALLABLE_FUNCTION:
            return functionToString((SyrFunction *)callable);
    }
}