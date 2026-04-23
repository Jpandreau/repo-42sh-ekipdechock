/*
** EPITECH PROJECT, 2026
** actions_cmd
** File description:
** actions_cmd
*/

#include "my.h"
#include "base.h"
#include "buildin.h"
#include "shell.h"

int actions_cmd_args(char **args, shell_t *shell)
{
    if (args == NULL || args[0] == NULL)
        return 84;
    if (split_buildin(args[0]))
        return run_buildin_shell(args, shell);
    if (buildin(args[0]))
        return run_buildin_args(args, &shell->env);
    return exec_cmd_args(args, shell->env);
}

int actions_cmd_args_nofork(char **args, shell_t *shell)
{
    if (args == NULL || args[0] == NULL)
        return 84;
    if (split_buildin(args[0]))
        return run_buildin_shell(args, shell);
    if (buildin(args[0]))
        return run_buildin_args(args, &shell->env);
    return exec_cmd_args_nofork(args, shell->env);
}
