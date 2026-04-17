/*
** EPITECH PROJECT, 2026
** 42sh
** File description:
** shell structure
*/
#ifndef SHELL_H
    #define SHELL_H

    #include "base.h"

typedef struct shell_s {
    char **env;
    char **locals;
    char **aliases;
    int exit_code;
    int running;
} shell_t;

char *get_local(char **locals, char *key);
int set_local(char ***locals, char *key, char *value);
int unset_local(char ***locals, char *key);
int set_builtin_args(char **args, char ***locals);
int unset_builtin_args(char **args, char ***locals);
char *expand_variables(char *str, shell_t *shell);

#endif
