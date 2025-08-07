#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/interpreter.h"
#include "../include/expr.h"
#include "../include/statement.h"
#include "../util/vector.h"
#include "../include/environment.h"

Expr *visitGroupingExpr(Expr *expr);
Expr *visitUnaryExpr(Expr *expr);
Expr *visitBinaryExpr(Expr* expr);
Expr *visitLetExpr(Expr *expr);

struct {
    Env *environment;
} interpreter;

void init() {
    interpreter.environment = env_create(NULL);
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

Expr *visitExpr(Expr *expr) {
    if (expr == NULL) {
        fprintf(stderr, "visitExpr: expr is NULL\n");
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
        case EXPR_BINARY:
            return visitBinaryExpr(expr);
        case EXPR_GROUPING:
            return visitGroupingExpr(expr);
        case EXPR_LET:
            return visitLetExpr(expr);
        default:
            fprintf(stderr, "visitExpr: unknown expr type %d\n", expr->type);
            return make_none_expr();
    }
}

Expr *visitGroupingExpr(Expr *expr) {
    return visitExpr(expr);
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
    Expr* rhs = visitExpr(expr->unary.rhs);

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
        default:
            break;
    }

    return rhs;
}

Expr *visitLetExpr(Expr *expr) {
    return env_get(interpreter.environment, expr->literal.token);
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
    Expr* lhs = visitExpr(expr->binary.lhs);
    Expr* rhs = visitExpr(expr->binary.rhs);
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
        default:
            return "unknown result";
    }
}

void *interpret_expr(Expr *expr) {
    Expr *result = visitExpr(expr);

    printf("%s", stringify(result));
    return result;
}

void visitExpressionStatement(ExprStmt *stmt);
void visitPrintStatement(PrintStmt *stmt);
void visitLetStatement(LetStmt *stmt);

void execute_stmt(Stmt *stmt) {
    switch (stmt->type) {
        case STMT_EXPRESSION:
            visitExpressionStatement((ExprStmt *)stmt);
            break;
        case STMT_PRINT: 
            visitPrintStatement((PrintStmt *)stmt);
            break;
        case STMT_LET:
            visitLetStatement((LetStmt *)stmt);
        default:
            break;
    }
}

void interpret_stmt(Vector* statements) {
    init();
    size_t num_stmt = vector_size(statements);
    for (size_t i = 0; i < num_stmt; i++) {
        execute_stmt(vector_at(statements, i));
    }
}

void visitExpressionStatement(ExprStmt *stmt) {
    visitExpr(stmt->expression);
}

void visitPrintStatement(PrintStmt *stmt) {
    Expr *value = visitExpr(stmt->expression);
    printf("%s", stringify(value));
}

void visitLetStatement(LetStmt *stmt) {
    Expr* value = NULL;
    if (stmt->initialiser != NULL) {
        value = visitExpr(stmt->initialiser);
    }

    env_define(interpreter.environment, stmt->name->value, value);
}
