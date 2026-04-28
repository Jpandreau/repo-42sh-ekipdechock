/*
** EPITECH PROJECT, 2026
** tokenize_extensions_impl
** File description:
** Implementation of extended tokenizer features
*/

#include "base.h"
#include "tokenize_extensions.h"

int is_inhibitor_char(char c, char quote)
{
    if (quote == '\'')
        return (c != '\'');
    if (quote == '"')
        return (c != '$' && c != '`' && c != '\\' && c != '"');
    return 0;
}

int is_glob_pattern(char *token)
{
    int i;

    if (token == NULL)
        return 0;
    i = 0;
    while (token[i]) {
        if (token[i] == '*' || token[i] == '?' || token[i] == '[')
            return 1;
        i++;
    }
    return 0;
}
