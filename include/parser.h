#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "lexer.h"

typedef struct {
    Token *tokens;
    Token *current;
    bool eof;
} Parser;

#endif