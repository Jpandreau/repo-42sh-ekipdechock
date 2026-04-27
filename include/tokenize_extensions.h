/*
** EPITECH PROJECT, 2026
** tokenize_extensions
** File description:
** Extended tokenizer features for globbing, inhibitors, and substitutions
*/

#ifndef TOKENIZE_EXTENSIONS_H
    #define TOKENIZE_EXTENSIONS_H

    #include "small_headers.h"

typedef enum
{
    TOKEN_TYPE_WORD,
    TOKEN_TYPE_SINGLE_QUOTE,
    TOKEN_TYPE_DOUBLE_QUOTE,
    TOKEN_TYPE_BACKTICK,
    TOKEN_TYPE_GLOB_STAR,
    TOKEN_TYPE_GLOB_QUESTION,
    TOKEN_TYPE_GLOB_BRACKET,
    TOKEN_TYPE_OPERATOR,
    TOKEN_TYPE_SUBSHELL_OPEN,
    TOKEN_TYPE_SUBSHELL_CLOSE,
} token_type_t;

typedef struct token_s
{
    token_type_t type;
    char *value;
} token_t;

int is_inhibitor_char(char c, char quote);
int is_glob_pattern(char *token);

#endif
