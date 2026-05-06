/*
** EPITECH PROJECT, 2026
** 42sh
** File description:
** Extended tokenizer features
*/

#ifndef TOKENIZE_EXTENSIONS_H
    #define TOKENIZE_EXTENSIONS_H

    #include "small_headers.h"

int is_inhibitor_char(char c, char quote);
int is_glob_pattern(char *token);
int expand_glob_pattern(char *pattern, char ***results);

#endif
