#include <stdio.h>
#include <stdlib.h>
#include "string.h"

#include "../include/interpreter.h"
#include "../include/expr.h"

Expr *visitGroupingExpr(Expr *expr);
Expr *visitUnaryExpr(Expr *expr);
Expr *visitBinaryExpr(Expr* expr);

ExprType visitLiteralExpr(Expr expr) {
    return expr.type;
}

Expr *visitExpr(Expr *expr) {
    switch (expr->type) {
        case EXPR_NUMBER:
        case EXPR_BOOL:
        case EXPR_NIL:
        case EXPR_STRING:
        case EXPR_UNARY:
            return visitUnaryExpr(expr);
        case EXPR_BINARY:
            return visitBinaryExpr(expr);
        case EXPR_GROUPING:
            return visitGroupingExpr(expr);
        default: 
            return make_none_expr();

    }
}

Expr *visitGroupingExpr(Expr *expr) {
    return visitExpr(expr);
};

bool isTruthy(Expr *expr) {
    if (expr->type == EXPR_NIL) return false;
    if (expr->type == EXPR_BOOL) return expr->literal.boolean;
    return true;
}

Expr  *visitUnaryExpr(Expr *expr) {
    Expr* rhs = visitExpr(expr->unary.rhs);

    switch (expr->unary.op->type) {
        case _MINUS:
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

Expr *visitBinaryExpr(Expr* expr) {
    Expr* lhs = visitExpr(expr->binary.lhs);
    Expr* rhs = visitExpr(expr->binary.rhs);
    Expr* result;
    bool truth;
    // double numResult;

    switch (expr->binary.op->type) {
        case _MINUS:
            result = make_num_expr(
                lhs->literal.number - rhs->literal.number, makeExprToken(_NUMBER, "1"));
            return result;
        case _SLASH:
            result = make_num_expr(
                lhs->literal.number / rhs->literal.number, makeExprToken(_NUMBER, "1"));
            return result;
        case _STAR:
            result = make_num_expr(
                lhs->literal.number * rhs->literal.number, makeExprToken(_NUMBER, "1"));
            return result;
        case _PLUS:
            if (lhs->type == EXPR_NUMBER && rhs->type == EXPR_NUMBER) {
                result = make_num_expr(lhs->literal.number + rhs->literal.number, makeExprToken(_NUMBER, "1"));
            }

            if (lhs->type == EXPR_STRING && rhs->type == EXPR_STRING) {
                char* combined = malloc(strlen(lhs->literal.string) + strlen(rhs->literal.string) + 1);
                strcpy(combined, lhs->literal.string);
                strcat(combined, rhs->literal.string);
                result = make_string_expr(combined, makeExprToken(_STRING, "combinedString"));
            }
            return result;
        case _GREATER:
            truth = lhs->literal.number > rhs->literal.number;
            result = make_bool_expr(truth, makeExprToken(truth ? _TRUE : _FALSE, "bool"));
            return result;
        case _GREATER_THAN:
            truth = lhs->literal.number >= rhs->literal.number;
            result = make_bool_expr(truth, makeExprToken(truth ? _TRUE : _FALSE, "bool"));
            return result;
        case _LESS:
            truth = lhs->literal.number < rhs->literal.number;
            result = make_bool_expr(truth, makeExprToken(truth ? _TRUE : _FALSE, "bool"));
            return result;
        case _LESS_THAN:
            truth = lhs->literal.number <= rhs->literal.number;
            result = make_bool_expr(truth, makeExprToken(truth ? _TRUE : _FALSE, "bool"));
            return result;
        case _EQUAL_EQUAL:
            truth = lhs->literal.number == rhs->literal.number;
            result = make_bool_expr(truth, makeExprToken(truth ? _TRUE : _FALSE, "bool"));
            return result;
        case _BANG_EQUAL:
            truth = lhs->literal.number != rhs->literal.number;
            result = make_bool_expr(truth, makeExprToken(truth ? _TRUE : _FALSE, "bool"));
            return result;
        case _AND:
            truth = lhs->literal.number && rhs->literal.number;
            result = make_bool_expr(truth, makeExprToken(truth ? _TRUE : _FALSE, "bool"));
            return result;
        case _OR:
            truth = lhs->literal.number || rhs->literal.number;
            result = make_bool_expr(truth, makeExprToken(truth ? _TRUE : _FALSE, "bool"));
            return result;
        default:
            break;
    }
    return NULL;
}