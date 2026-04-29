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
    char **locals;
    char **aliases;
} shell_t;

char *get_local(char **locals, char *key);
int set_local(char ***locals, char *key, char *value);
int unset_local(char ***locals, char *key);
int set_builtin_args(char **args, char ***locals);
int unset_builtin_args(char **args, char ***locals);

char *get_alias(char **aliases, char *key);
int set_alias_entry(char ***aliases, char *key, char *value);
int alias_builtin_args(char **args, char ***aliases);
int unalias_builtin_args(char **args, char ***aliases);
int is_shell_builtin(char *cmd);
int run_shell_builtin(char **args, shell_t *shell);

int shell_init(shell_t *shell);
void shell_destroy(shell_t *shell);

char *expand_alias_in_line(char *line, char **aliases);
char *expand_vars_in_line(char *line, char **locals, char **env);

#endif
