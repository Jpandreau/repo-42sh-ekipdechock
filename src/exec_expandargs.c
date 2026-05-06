/*
** EPITECH PROJECT, 2026
** 42sh
** File description:
** expand glob patterns in args before exec
*/

#include "base.h"
#include "tokenize_extensions.h"
#include "expandargs.h"

static char **expand_one_token(char **result, char *token)
{
    char **expanded = NULL;
    char **tmp = NULL;
    int i = 0;

    if (!is_glob_pattern(token))
        return my_array_add(result, my_strdup(token));
    if (expand_glob_pattern(token, &expanded) != 0)
        return my_array_add(result, my_strdup(token));
    while (expanded[i]) {
        tmp = my_array_add(result, my_strdup(expanded[i]));
        if (tmp)
            result = tmp;
        i++;
    }
    free_array(expanded);
    return result;
}

char **expand_glob_args(char **args)
{
    char **result = malloc(sizeof(char *) * 1);
    int i = 0;

    if (!result)
        return NULL;
    result[0] = NULL;
    while (args[i]) {
        result = expand_one_token(result, args[i]);
        if (!result)
            return NULL;
        i++;
    }
    return result;
}

static char **add_backtick_result(char **result, char **words)
{
    char **tmp = NULL;
    int i = 0;

    while (words && words[i]) {
        tmp = my_array_add(result, my_strdup(words[i]));
        if (tmp)
            result = tmp;
        i++;
    }
    return result;
}

char **expand_backtick_args(char **args, token_type_t *arg_types, char ***env)
{
    char **result = malloc(sizeof(char *) * 1);
    char **words = NULL;
    int i = 0;

    if (!result)
        return NULL;
    result[0] = NULL;
    while (args[i]) {
        if (arg_types && arg_types[i] == TOKEN_TYPE_BACKTICK) {
            words = expand_backtick(args[i], env);
            result = add_backtick_result(result, words);
            free_array(words);
        } else {
            result = my_array_add(result, my_strdup(args[i]));
        }
        if (!result)
            return NULL;
        i++;
    }
    return result;
}
