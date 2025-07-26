#include <stdio.h>
#include <stdlib.h>

#include "../include/lexer.h"
#include "../include/parser.h"
#include "../include/expr.h"

void run(char* source) {
    initLexer(source);
    scanTokens(); // gives tokens

    // Parser parser = initParser(&lexer);
    // Expr *expression = parse(source)
}