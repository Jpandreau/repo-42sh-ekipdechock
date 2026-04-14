/*
** EPITECH PROJECT, 2026
** exec_launch_format_arg
** File description:
** exec_launch_format_arg
*/

#include "my.h"
#include "base.h"

int exec_launch_format_arg(char *cmd, char *arg, char **env)
{
    char **arg_array = my_str_to_word_array(arg, ' ');
    char **final_args = NULL;

    if (arg != NULL && arg_array == NULL)
        return 84;
    final_args = exec_build_args_dup(cmd, arg_array);
    if (final_args == NULL)
        return exec_free_args(arg_array);
    if (exec_can_exec(cmd) != 0)
        return exec_free_args(final_args);
    return my_execve(cmd, final_args, env);
}
