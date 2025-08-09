#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "../include/parser.h"
#include "../include/expr.h"
#include "../include/debug.h"
#include "../util/vector.h"
#include "../include/statement.h"
#include "../include/lexer.h"

Parser initParser(Token *ts) {
    Parser parser;
    parser.tokens = ts;
    parser.current = parser.tokens;
    parser.eof = false;
    return parser;
}

Token parser_peek(Parser *parser) {
    return *parser->current;
}

bool check(Parser *parser, TokenType type) {
    return parser_peek(parser).type == type;
}

bool parser_is_at_end(Parser *parser) {
    return check(parser, _EOF);
}

Token *parser_advance(Parser *parser) {
    Token *token = parser->current++;
    parser->eof = (parser->current->type == _EOF);
    return token;
}

bool parser_check(Parser *parser, TokenType type) {
    return !parser->eof && parser->current->type == type;
}

Token *consume(Parser *parser, TokenType type, char* message) {
    if (parser_check(parser, type)) {
        return parser_advance(parser);
    } else {
        fprintf(stderr, "[line %d] Error: %s\n", parser->current->line, message);
        exit(EXIT_FAILURE);
    }
}

bool parser_match(Parser *parser, int count, ...) {
    va_list token_types;
    va_start(token_types, count);
    for (int i = 0; i < count; i++) {
        TokenType tt = va_arg(token_types, TokenType);
        if (parser_check(parser, tt)) {
            va_end(token_types);      // <- important
            parser_advance(parser);
            return true;
        }
    }
    va_end(token_types);
    return false;
}


// bool parser_match(Parser *parser, TokenType type) {
//     if (!check(parser, type)) return false;
//     parser_advance(parser);
//     return true;
// }

// a lot of overlap so defined first

Expr *expression(Parser *parser);
Expr *comparison(Parser *parser);
Expr *equality(Parser *parser);
Expr *addition(Parser *parser);
Expr *multiplication(Parser *parser);
Expr *unary(Parser *parser);
Expr *primary(Parser *parser);
Expr *assignment(Parser *parser);
Expr *or(Parser *parser);
Expr *and(Parser *parser);

Expr *expression(Parser *parser) {
    return assignment(parser);
}

Expr *equality(Parser *parser) {
    Expr *expr = comparison(parser);
    while (parser_match(parser, 2, _EQUAL_EQUAL, _BANG_EQUAL)) {
        Token *op  = parser->current-1;
        Expr *rhs = comparison(parser);
        expr = make_binary_expr(op, expr, rhs);
    }
    return expr;
}

Expr *assignment(Parser *parser) {
    Expr *expr = or(parser);
    
    if (parser_match(parser, 1, _EQUAL)) {
        Token *equals = parser->current-1;
        Expr *value = assignment(parser);

        if (expr->type == EXPR_LET) {
            Token *name = ((Expr *)expr)->literal.token;
            return make_assign_expr(name, value);
        }

        fprintf(stderr, "%s is an invalid assignment target\n", equals->value);
        exit(EXIT_FAILURE);
    }

    return expr;    
}

Expr *or(Parser *parser) {
    Expr *expr = and(parser);

    while (parser_match(parser, 1, _OR)) {
        Token *op = parser->current-1;
        Expr *right = and(parser);
        expr = make_logical_expr(expr, op, right);
    }

    return expr;
}

Expr *and(Parser *parser) {
    Expr *expr = equality(parser);
    while (parser_match(parser, 1, _AND)) {
        Token *op = parser->current-1;
        Expr *right = equality(parser);
        expr = make_logical_expr(expr, op, right);
    }

    return expr;
}

Expr *comparison(Parser *parser)
{
    Expr *e = addition(parser);
    while (parser_match(parser, 4, _GREATER, _GREATER_EQUAL, _LESS, _LESS_EQUAL)) {
        Token *op = parser->current-1;
        Expr *rhs = addition(parser);
        e = make_binary_expr(op, e, rhs);
    }
    return e;
}

Expr *unary(Parser *parser) {
    if (parser_match(parser, 2, _BANG, _MINUS)) {
        Token* op = parser->current-1;
        Expr *rhs = unary(parser);
        return make_unary_expr(op, rhs);
    }

    return primary(parser);
}

Expr *primary(Parser *parser) {
    Token *t = parser->current;
    double number;
    
    if (parser_match(parser, 1, _NIL))    return make_nil_expr(parser->current-1);
    if (parser_match(parser, 1, _FALSE))  return make_bool_expr(false, parser->current-1);
    if (parser_match(parser, 1, _TRUE))   return make_bool_expr(true, parser->current-1);
    if (parser_match(parser, 1, _NUMBER)) {
        sscanf(t->value, "%lf", &number);
        return make_num_expr(number, parser->current-1);
    }
    if (parser_match(parser, 1, _STRING)) return make_string_expr(t->value, parser->current-1);
    if (parser_match(parser, 1, _IDENTIFIER)) return make_let_expr(t->value, parser->current-1);
    if (parser_match(parser, 1, _LEFT_PAR)) {
        Expr *e = expression(parser);
        consume(parser, _RIGHT_PAR, "Expected ')' after expression. Silly!");
        return make_grouping_expr(e);
    }

    printf("unexpected token ");
    print_token(t);
    return &NoneExpr;

}

Expr *multiplication(Parser *parser) {
    Expr *e = unary(parser);
    while (parser_match(parser, 2, _SLASH, _STAR)) {
        Token *op = parser->current-1;
        Expr *rhs = unary(parser);
        e = make_binary_expr(op, e, rhs);
    }
    return e;
}

Expr *addition(Parser *parser) {
    Expr *e = multiplication(parser);
    while (parser_match(parser, 2, _PLUS, _MINUS)) {
        Token *op = parser->current-1;
        Expr *rhs = multiplication(parser);
        e = make_binary_expr(op, e, rhs);
    }
    return e;
}

void synchronise(Parser *parser) {
    parser_advance(parser);

    while (!parser->eof) {
        if (parser->current[-1].type == _SEMICOLON) return;
        switch (parser->current->type) {
            case _LET:
            case _DEF:
            case _PRINT:
            case _WHILE:
            case _FOR:
            case _IF:
            case _RETURN:
                return;
            default:
                break;
        }
        parser_advance(parser);
    }
}

Expr *parse_expr(Parser* parser) {
    if (parser->tokens->type == _NONE) {
        return NULL;
    }
    
    Expr *e = expression(parser);
    return e;
}

// declared first to use later

Stmt *statement(Parser *parser);
Stmt *printStatement(Parser *parser);
Stmt *expressionStatement(Parser *parser);
Stmt *declaration(Parser *parser);
Stmt *letDeclaration(Parser *parser);
Vector *block(Parser *parser);
Stmt *ifStatement(Parser *parser);
Stmt *whileStatement(Parser *parser);
Stmt *forStatement(Parser *parser);

Vector *parse_stmt(Parser *parser) {
    Vector *statements = vector_construct();
    while (!parser_is_at_end(parser)) {

        vector_push_back(statements, declaration(parser));
    }
    return statements;
}

Stmt *declaration(Parser *parser) {
    if (parser_match(parser, 1 , _LET)) return letDeclaration(parser);
    return statement(parser);
}

Stmt *statement(Parser *parser) {
    if (parser_match(parser, 1, _PRINT)) return printStatement(parser);
    // if (parser_match(parser, 1, _LET)) return letDeclaration(parser);
    if (parser_match(parser, 1, _LEFT_CURLY)) return (Stmt *)make_block_statement(block(parser));
    if (parser_match(parser, 1, _IF)) return ifStatement(parser);
    if (parser_match(parser, 1, _WHILE)) return whileStatement(parser);
    if (parser_match(parser, 1, _FOR)) return forStatement(parser);
    return expressionStatement(parser);
}

Stmt *ifStatement(Parser *parser) {
    consume(parser, _LEFT_PAR, "Expect '(' after 'if'.");
    Expr *condition = expression(parser);
    consume(parser, _RIGHT_PAR, "Expect ')' after condition.");

    Stmt *thenBranch = statement(parser);
    Stmt *elseBranch = NULL;
    if (parser_match(parser, 1, _ELSE)) {
        elseBranch = statement(parser);
    }

    return (Stmt *)make_if_statement(condition, thenBranch, elseBranch);

}

Stmt *whileStatement(Parser *parser) {
    consume(parser, _LEFT_PAR, "Expect '(' after 'while'.");
    Expr *condition = expression(parser);
    consume(parser, _RIGHT_PAR, "Expect ')' after condition.");

    Stmt *body = statement(parser);

    return (Stmt *)make_while_statement(condition, body);
}

Stmt *forStatement(Parser *parser) {
    consume(parser, _LEFT_PAR, "Expect '(' after 'for'.");

    Stmt *initialiser;
    if (parser_match(parser, 1, _SEMICOLON)) {
        initialiser = NULL;
    } else if (parser_match(parser, 1, _LET)) {
        initialiser = letDeclaration(parser);
    } else {
        initialiser = expressionStatement(parser);
    }

    Expr *condition = NULL;
    if (!parser_check(parser, _SEMICOLON)) {
        condition = expression(parser);
    }
    consume(parser, _SEMICOLON, "Expect ';' after loop condition.");

    Expr *increment = NULL;
    if (!parser_check(parser, _RIGHT_PAR)) {
        increment = expression(parser);
    }
    consume(parser, _RIGHT_PAR, "Expect ')' after for clauses.");

    Stmt *body = statement(parser);

    if (increment != NULL) {
        Vector *statements = vector_construct();
        vector_push_back(statements, body);
        vector_push_back(statements, make_expression_statement(increment));
        body = (Stmt *)make_block_statement(statements);
    }

    if (condition == NULL) condition = make_bool_expr(true, parser->current);

    body = (Stmt *)make_while_statement(condition, body);

    if (initialiser != NULL) {
        Vector *statements = vector_construct();
        vector_push_back(statements, initialiser);
        vector_push_back(statements, body);
        body = (Stmt *)make_block_statement(statements);
    }

    return body;
}

Stmt *printStatement(Parser *parser) {
    Expr *value = expression(parser);
    consume(parser, _SEMICOLON, "Expect ';' after value.");
    return (Stmt *)make_print_statement(value);
}

Stmt *expressionStatement(Parser *parser) {
    Expr *expr = expression(parser);
    consume(parser, _SEMICOLON, "Expect ';' after value.");
    return (Stmt *)make_expression_statement(expr);
}

Stmt *letDeclaration(Parser *parser) {
    Token *name = consume(parser, _IDENTIFIER, "Expect variable name");
    Expr *initialiser = NULL;

    if (parser_match(parser, 1, _EQUAL)) {
        initialiser = expression(parser);
    }
    consume(parser, _SEMICOLON, "Expect ';' after value.");
    return (Stmt *)make_let_statement(name, initialiser);
}

Vector *block(Parser *parser) {
    Vector *statements = vector_construct();

    while (!parser_check(parser, _RIGHT_CURLY) && !parser_is_at_end(parser)) {
        vector_push_back(statements, declaration(parser));
    }

    consume(parser, _RIGHT_CURLY, "Expect '}' after block.");
    return statements;
};

