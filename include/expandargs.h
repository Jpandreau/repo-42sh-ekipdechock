/*
** EPITECH PROJECT, 2026
** 42sh
** File description:
** expand glob args header
*/

#ifndef EXPANDARGS_H
    #define EXPANDARGS_H

    #include "base.h"
    #include "small_headers.h"

char **expand_glob_args(char **args);
char **expand_backtick(char *cmd, char ***env);
char **expand_backtick_args(char **args, token_type_t *arg_types, char ***env);

#endif /* !EXPANDARGS_H */
