/*
** EPITECH PROJECT, 2026
** actions_cmd
** File description:
** actions_cmd
*/

#include "my.h"
#include "base.h"
#include "buildin.h"

int actions_cmd_args(char **args, char ***env, history_t *history)
{
    if (args == NULL || args[0] == NULL)
        return 84;
    if (buildin(args[0]))
        return run_buildin_args(args, env, history);
    return exec_cmd_args(args, *env);
}

int actions_cmd_args_nofork(char **args, char ***env, history_t *history)
{
    if (args == NULL || args[0] == NULL)
        return 84;
    if (buildin(args[0]))
        return run_buildin_args(args, env, history);
    return exec_cmd_args_nofork(args, *env);
}
