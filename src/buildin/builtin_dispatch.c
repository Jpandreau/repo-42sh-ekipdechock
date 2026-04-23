/*
** EPITECH PROJECT, 2026
** builtin_dispatch
** File description:
** builtin selection and dispatch
*/

#include "base.h"
#include "buildin.h"
#include "shell.h"

static int split_buildin(char *cmd)
{
    if (cmd == NULL)
        return 0;
    if (!my_strcmp(cmd, "set"))
        return 1;
    if (!my_strcmp(cmd, "unset"))
        return 1;
    if (!my_strcmp(cmd, "alias"))
        return 1;
    if (!my_strcmp(cmd, "unalias"))
        return 1;
    return 0;
}

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
    return split_buildin(cmd);
}

int run_buildin_args(char **args, char ***env)
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
    return 0;
}

int run_buildin_shell(char **args, shell_t *shell)
{
    if (args == NULL || args[0] == NULL || shell == NULL)
        return 84;
    if (!my_strcmp(args[0], "set"))
        return set_builtin_args(args, &shell->locals);
    if (!my_strcmp(args[0], "unset"))
        return unset_builtin_args(args, &shell->locals);
    if (!my_strcmp(args[0], "alias"))
        return alias_builtin_args(args, &shell->aliases);
    if (!my_strcmp(args[0], "unalias"))
        return unalias_builtin_args(args, &shell->aliases);
    return 0;
}
