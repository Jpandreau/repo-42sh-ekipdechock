/*
** EPITECH PROJECT, 2026
** exec_free
** File description:
** exec_free
*/

#include "my.h"
#include "base.h"

int exec_free_cmd(char *cmd)
{
    free(cmd);
    return 84;
}

int exec_free_args(char **args)
{
    free_array(args);
    return 84;
}

int exec_free_args_cmd(char **args, char *cmd)
{
    free_array(args);
    free(cmd);
    return 84;
}
