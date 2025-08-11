#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/interpreter.h"
#include "../include/expr.h"
#include "../include/statement.h"
#include "../util/vector.h"
#include "../include/environment.h"
#include "../include/syrCallable.h"
#include "../include/debug.h"
#include "../include/syrFunction.h"

Expr *visitGroupingExpr(Expr *expr);
Expr *visitUnaryExpr(Expr *expr);
Expr *visitBinaryExpr(Expr* expr);
Expr *visitLetExpr(Expr *expr);
Expr *visitAssignExpr(Expr *expr);
Expr *evaluateExpr(Expr *expr);
Expr *visitPostfixExpr(Expr *expr);
Expr *visitCallExpr(Expr *expr);

Interpreter interpreter;

void init() {
    interpreter.global = env_create(NULL);
    interpreter.environment = interpreter.global;
}

void checkNumberOperand(Token *op, Expr *rhs) {
    if (rhs->type == EXPR_NUMBER) return;
    fprintf(stderr, "[line %d] Runtime error: %s\n", op->line, "Operand must be a number");
    exit(EXIT_FAILURE);
}

void checkNumberOperands(Token *op, Expr *lhs, Expr *rhs) {
    if (lhs->type == EXPR_NUMBER && rhs->type == EXPR_NUMBER) return;
    fprintf(stderr, "[line %d] Runtime error: %s\n", op->line, "Operands must be a number");
    exit(EXIT_FAILURE);
};

bool isEqual(Expr *lhs, Expr *rhs) {
    if (lhs->type != rhs->type) return false;

    switch (lhs->type) {
        case EXPR_NIL: return true;
        case EXPR_BOOL: return lhs->literal.boolean == rhs->literal.boolean;
        case EXPR_NUMBER: return lhs->literal.number == rhs->literal.number;
        case EXPR_STRING: return strcmp(lhs->literal.string, rhs->literal.string) == 0;
        default: return false;
    }
}

bool isTruthy(Expr *expr) {
    if (expr->type == EXPR_NIL) return false;
    if (expr->type == EXPR_BOOL) return expr->literal.boolean;
    return true;
}

ExprType visitLiteralExpr(Expr expr) {
    return expr.type;
}

Expr *visitLogicalExpr(Expr *expr) {
    Expr* left = expr->logical.lhs;

    if (expr->logical.op->type == _OR) {
        if (isTruthy(left)) return left;
    } else {
        if (!isTruthy(left)) return left;
    }

    return evaluateExpr(expr->logical.rhs);
}

Expr *evaluateExpr(Expr *expr) {
    if (expr == NULL) {
        fprintf(stderr, "evaluateExpr: expr is NULL\n");
        return NULL;
    }
    switch (expr->type) {
        case EXPR_NUMBER:
        case EXPR_BOOL:
        case EXPR_NIL:
        case EXPR_STRING:
            return expr;
        case EXPR_UNARY:
            return visitUnaryExpr(expr);
        case EXPR_POSTFIX:
            return visitPostfixExpr(expr);
        case EXPR_BINARY:
            return visitBinaryExpr(expr);
        case EXPR_GROUPING:
            return visitGroupingExpr(expr);
        case EXPR_LET:
            return visitLetExpr(expr);
        case EXPR_ASSIGN:
            return visitAssignExpr(expr);
        case EXPR_LOGICAL:
            return visitLogicalExpr(expr);
        case EXPR_CALL:
            return visitCallExpr(expr);
        default:
            fprintf(stderr, "evaluateExpr: unknown expr type %d\n", expr->type);
            return make_none_expr();
    }
}

Expr *visitGroupingExpr(Expr *expr) {
    return evaluateExpr(expr->grouping);
};

Expr  *visitUnaryExpr(Expr *expr) {
    if (expr == NULL) {
        fprintf(stderr, "visitUnaryExpr: expr is NULL\n");
        return NULL;
    }
    if (expr->unary.rhs == NULL) {
        fprintf(stderr, "visitUnaryExpr: expr->unary.rhs is NULL\n");
        return NULL;
    }
    Expr* rhs = evaluateExpr(expr->unary.rhs);

    if (expr->unary.op == NULL) {
        fprintf(stderr, "visitUnaryExpr: expr->unary.op is NULL\n");
        return rhs;
    }

    switch (expr->unary.op->type) {
        case _MINUS:
            checkNumberOperand(expr->unary.op, rhs);
            rhs->literal.number *= -1;
            break;
        case _BANG:
            rhs->literal.boolean = !isTruthy(rhs);
            break;
        case _PLUS_PLUS:
            checkNumberOperand(expr->unary.op, rhs);
            ++(rhs->literal.number);
            break;
        case _MINUS_MINUS:
            checkNumberOperand(expr->unary.op, rhs);
            --(rhs->literal.number);
            break;
        default:
            break;
    }

    return rhs;
}

Expr *visitPostfixExpr(Expr *expr) {
    if (expr == NULL) {
        fprintf(stderr, "visitPostfixExpr: expr is NULL\n");
        return NULL;
    }
    if (expr->postfix.lhs == NULL) {
        fprintf(stderr, "visitPostfixExpr: expr->postfix.lhs is NULL\n");
        return NULL;
    }
    Expr* lhs = evaluateExpr(expr->postfix.lhs);

    if (expr->postfix.op == NULL) {
        fprintf(stderr, "visitPostfixExpr: expr->postfix.op is NULL\n");
        return lhs;
    }

    switch (expr->postfix.op->type) {
        case _PLUS_PLUS:
            checkNumberOperand(expr->postfix.op, lhs);
            (lhs->literal.number)++;
            break;
        case _MINUS_MINUS:
            checkNumberOperand(expr->postfix.op, lhs);
            (lhs->literal.number)--;
            break;
        default:
            break;
    }

    return lhs;
}


Expr *visitLetExpr(Expr *expr) {
    return env_get(interpreter.environment, expr->literal.token);
}

Expr *visitAssignExpr(Expr *expr) {
    Expr* value = evaluateExpr(expr->value);
    env_assign(interpreter.environment, expr->literal.token, value);
    return value;
}

Expr *visitBinaryExpr(Expr* expr) {
    if (expr == NULL) {
        fprintf(stderr, "visitBinaryExpr: expr is NULL\n");
        return NULL;
    }
    if (expr->binary.lhs == NULL) {
        fprintf(stderr, "visitBinaryExpr: expr->binary.lhs is NULL\n");
        return NULL;
    }
    if (expr->binary.rhs == NULL) {
        fprintf(stderr, "visitBinaryExpr: expr->binary.rhs is NULL\n");
        return NULL;
    }
    if (expr->binary.op == NULL) {
        fprintf(stderr, "visitBinaryExpr: expr->binary.op is NULL\n");
        return NULL;
    }
    Expr* lhs = evaluateExpr(expr->binary.lhs);
    Expr* rhs = evaluateExpr(expr->binary.rhs);
    Expr* result;
    bool truth;
    // double numResult;

    switch (expr->binary.op->type) {
        case _MINUS:
            checkNumberOperands(expr->binary.op, lhs, rhs);
            result = make_num_expr(lhs->literal.number - rhs->literal.number, expr->binary.op);
            return result;
        case _SLASH:
            checkNumberOperands(expr->binary.op, lhs, rhs);
            if (rhs->literal.number == 0) {
                fprintf(stderr, "[line %d] Runtime error: %s\n", expr->binary.op->line, "Division by zero");
                exit(EXIT_FAILURE);
            }
            result = make_num_expr(lhs->literal.number / rhs->literal.number, expr->binary.op);
            return result;
        case _STAR:
            checkNumberOperands(expr->binary.op, lhs, rhs);
            result = make_num_expr(lhs->literal.number * rhs->literal.number, expr->binary.op);
            return result;
        case _PLUS:
            if (lhs->type == EXPR_NUMBER && rhs->type == EXPR_NUMBER) {
                result = make_num_expr(lhs->literal.number + rhs->literal.number, expr->binary.op);
                return result;
            }

            if (lhs->type == EXPR_STRING && rhs->type == EXPR_STRING) {
                char* combined = malloc(strlen(lhs->literal.string) + strlen(rhs->literal.string) + 1);
                strcpy(combined, lhs->literal.string);
                strcat(combined, rhs->literal.string);
                result = make_string_expr(combined, expr->binary.op);
                return result;
            }

            fprintf(stderr, "[line %d] Runtime error: %s\n", expr->binary.op->line, "Operands must be two numbers or two strings");
            exit(EXIT_FAILURE);
        case _GREATER:
            checkNumberOperands(expr->binary.op, lhs, rhs);
            truth = lhs->literal.number > rhs->literal.number;
            result = make_bool_expr(truth, expr->binary.op);
            return result;
        case _GREATER_EQUAL:
            checkNumberOperands(expr->binary.op, lhs, rhs);
            truth = lhs->literal.number >= rhs->literal.number;
            result = make_bool_expr(truth, expr->binary.op);
            return result;
        case _LESS:
            checkNumberOperands(expr->binary.op, lhs, rhs);
            truth = lhs->literal.number < rhs->literal.number;
            result = make_bool_expr(truth, expr->binary.op);
            return result;
        case _LESS_EQUAL:
            checkNumberOperands(expr->binary.op, lhs, rhs);
            truth = lhs->literal.number <= rhs->literal.number;
            result = make_bool_expr(truth, expr->binary.op);
            return result;
        case _EQUAL_EQUAL:
            truth = isEqual(lhs, rhs);
            result = make_bool_expr(truth, expr->binary.op);
            return result;
        case _BANG_EQUAL:
            truth = !isEqual(lhs, rhs);
            result = make_bool_expr(truth, expr->binary.op);
            return result;
        case _AND:
            truth = isTruthy(lhs) && isTruthy(rhs); // for int 0 this is not correct
            result = make_bool_expr(truth, expr->binary.op);
            return result;
        case _OR:
            truth = isTruthy(lhs) || isTruthy(rhs); // for int 0 this is not correct
            result = make_bool_expr(truth, expr->binary.op);
            return result;
        default:
            fprintf(stderr, "visitBinaryExpr: unknown binary op type %d\n", expr->binary.op->type);
            break;
    }
    return NULL;
}

Expr *visitCallExpr(Expr *expr) {
    Expr *callee = evaluateExpr(expr->call.callee);

    Vector *arguments = vector_construct();
    size_t size = vector_size(expr->call.arguments);
    for (size_t i=0; i < size; i++) {
        vector_push_back(arguments, evaluateExpr(vector_at(expr->call.arguments, i)));
    }

    if (callee->type != EXPR_SYR_CALLABLE) {
        interpret_error(expr->call.paren, "Can only call functions and classes.");
    }

    SyrCallable *function = callee->SyrCallable.callable;

    size_t arity = syrCallableArity(function);

    if (size != arity) {
        interpret_error(expr->call.paren, "Expected %zu arguments but got %zu.", arity, size);
    }
    return syrCallableCall(function, arguments);
}

char *stringify(Expr *result) {
    // char *str;
    if (result == NULL) {
        fprintf(stderr, "Interpretation failed: result is NULL\n");
        return NULL;
    }


    switch (result->type) {
        case EXPR_NUMBER: {
            // printf("%g\n", result->literal.number); break;

            char *str = malloc(sizeof(char) * 256);
            sprintf(str, "%g", result->literal.number);
            return str;
        }
        case EXPR_STRING:
            // printf("%s\n", result->literal.string); break;
            return result->literal.string;
        case EXPR_BOOL:
            // printf(result->literal.boolean ? "true\n" : "false\n"); break;
            return result->literal.boolean ? "true" : "false";
        case EXPR_NIL:
            // printf("nil\n"); break;
            return "nil";
        case EXPR_SYR_CALLABLE:
            return syrCallableToString(result->SyrCallable.callable);
        default:
            return "unknown result";
    }
}

void *interpret_expr(Expr *expr) {
    Expr *result = evaluateExpr(expr);
    printf("%s", stringify(result));
    return result;
}

Expr *visitExpressionStatement(ExprStmt *stmt);
Expr *visitIfStatement(IfStmt *stmt);
Expr *visitPrintStatement(PrintStmt *stmt);
Expr *visitLetStatement(LetStmt *stmt);
Expr *visitBlockStatement(BlockStmt *stmt);
Expr *visitWhileStatement(WhileStmt *stmt);
Expr *visitFunctionStatement(FunctionStmt *stmt);

Expr *execute_stmt(Stmt *stmt) {
    switch (stmt->type) {
        case STMT_BLOCK:
            return visitBlockStatement((BlockStmt *)stmt);
        case STMT_EXPRESSION:
            return visitExpressionStatement((ExprStmt *)stmt);
        case STMT_PRINT: 
            return visitPrintStatement((PrintStmt *)stmt);
        case STMT_LET:
            return visitLetStatement((LetStmt *)stmt);
        case STMT_IF:
            return visitIfStatement((IfStmt *)stmt);
        case STMT_WHILE:
            return visitWhileStatement((WhileStmt *)stmt);
        case STMT_FUNCTION:
            return visitFunctionStatement((FunctionStmt *)stmt);
        default:
            return NULL;
    }
}

void interpret_stmt(Vector* statements) {
    init();
    size_t num_stmt = vector_size(statements);
    for (size_t i = 0; i < num_stmt; i++) {
        execute_stmt(vector_at(statements, i));
    }
}

Expr *visitExpressionStatement(ExprStmt *stmt) {
    evaluateExpr(stmt->expression);
    return NULL;
}

Expr *visitFunctionStatement(FunctionStmt *stmt) {
    SyrFunction *function = make_function(stmt, interpreter.environment, false);
    Expr *expr = make_syr_callable_expr((SyrCallable *)function);
    env_define(interpreter.environment, stmt->name->value, expr);
    
    return NULL;
}

Expr *visitIfStatement(IfStmt *stmt) {
    if (isTruthy(evaluateExpr(stmt->condition))) {
        execute_stmt(stmt->thenBranch);
    } else if (stmt->elseBranch != NULL) {
        execute_stmt(stmt->elseBranch);
    }
    return NULL;
}

Expr *visitWhileStatement(WhileStmt *stmt) {
    while (isTruthy(evaluateExpr(stmt->condition))) {
        execute_stmt(stmt->body);
    }

    return NULL;
}


Expr *visitPrintStatement(PrintStmt *stmt) {
    Expr *value = evaluateExpr(stmt->expression);
    printf("%s", stringify(value));
    return NULL;
}

Expr *visitLetStatement(LetStmt *stmt) {
    Expr* value = NULL;
    if (stmt->initialiser != NULL) {
        value = evaluateExpr(stmt->initialiser);
    }

    env_define(interpreter.environment, stmt->name->value, value);
    return NULL;
}

Expr *executeBlock(Vector *statements, Env *env) {
    Env *prev = interpreter.environment;
    interpreter.environment = env;
    Expr *result = NULL;

    size_t num = vector_size(statements);
    for (size_t i=0; i < num; i++) {
        result = execute_stmt(vector_at(statements, i));
        if (result != NULL) break;
    }
    interpreter.environment = prev;
    return result;
};

Expr *visitBlockStatement(BlockStmt *stmt) {
    return executeBlock(stmt->statements, env_create(interpreter.environment));
}