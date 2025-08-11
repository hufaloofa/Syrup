#include <stdlib.h>
#include <time.h>
#include "../include/syrCallable.h"
#include "../include/expr.h"
#include "../util/vector.h"
#include "../include/interpreter.h"
#include "../include/syrFunction.h"



size_t syrCallableArity(SyrCallable *callable) {
    switch (callable->type) {
        case SYR_CALLABLE_FUNCTION:
            return functionArity((SyrFunction *)callable);
        case SYR_CALLABLE_CLOCK:
            return 0;
    }   
    fprintf(stderr, "syrCallableArity: unknown callable type %d\n", callable->type);
    exit(EXIT_FAILURE);
}


Expr *syrCallableCall(SyrCallable *callable, Vector *arguments) {
    switch (callable->type) {
        case SYR_CALLABLE_FUNCTION:
            // printf("syrCallableCall: calling function\n");
            return functionCall((SyrFunction *)callable, arguments);
        case SYR_CALLABLE_CLOCK:
            return make_num_expr(time(NULL), NULL); 
    }
    fprintf(stderr, "syrCallableCall: unknown callable type %d\n", callable->type);
    exit(EXIT_FAILURE);
}

char *syrCallableToString(SyrCallable *callable) {
    switch (callable->type) {
    case SYR_CALLABLE_CLOCK:
            return "<native fn>";
    case SYR_CALLABLE_FUNCTION:
            return functionToString((SyrFunction *)callable);
    }
    fprintf(stderr, "syrCallableToString: unknown callable type %d\n", callable->type);
    exit(EXIT_FAILURE);
}