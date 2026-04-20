/*
** EPITECH PROJECT, 2026
** builtin_dispatch
** File description:
** builtin selection and dispatch
*/

#include "base.h"
#include "buildin.h"

int buildin(char *cmd)
{
    if (cmd == NULL)
        return 0;
    if (!my_strcmp(cmd, "cd"))
        return 1;
    if (!my_strcmp(cmd, "env"))
        return 1;
    if (!my_strcmp(cmd, "setenv"))
        return 1;
    if (!my_strcmp(cmd, "unsetenv"))
        return 1;
    if (!my_strcmp(cmd, "exit"))
        return 1;
    if (!my_strcmp(cmd, "history"))
        return 1;
    if (!my_strcmp(cmd, "fg"))
        return 1;
    if (!my_strcmp(cmd, "bg"))
        return 1;
    return 0;
}

int run_buildin_args(char **args, char ***env, history_t *history)
{
    if (args == NULL || args[0] == NULL || env == NULL)
        return 84;
    if (!my_strcmp(args[0], "cd"))
        return cd_buildin_args(args, env);
    if (!my_strcmp(args[0], "env"))
        return env_buildin_args(args, *env);
    if (!my_strcmp(args[0], "setenv"))
        return setenv_buildin_args(args, env);
    if (!my_strcmp(args[0], "unsetenv"))
        return unsetenv_buildin_args(args, env);
    if (!my_strcmp(args[0], "exit"))
        return exit_buildin_args(args);
    if (!my_strcmp(args[0], "history"))
        return history_buildin_args(args, history);
    if (!my_strcmp(args[0], "fg"))
        return fg_buildin_args(args, history);
    if (!my_strcmp(args[0], "bg"))
        return bg_buildin_args(args, history);
    return 0;
}
