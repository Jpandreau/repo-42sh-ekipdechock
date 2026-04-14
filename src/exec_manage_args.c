/*
** EPITECH PROJECT, 2026
** exec_manage_args
** File description:
** exec_manage_args
*/

#include "my.h"
#include "base.h"

char **exec_build_empty(char *cmd)
{
    char **new_args = malloc(sizeof(char *) * 2);

    if (new_args == NULL)
        return NULL;
    new_args[0] = cmd;
    new_args[1] = NULL;
    return new_args;
}

char **exec_alloc_args(char *cmd, char **args)
{
    int size = array_size(args);
    char **new_args = malloc(sizeof(char *) * (size + 2));

    if (new_args == NULL)
        return NULL;
    new_args[0] = cmd;
    new_args[size + 1] = NULL;
    return new_args;
}

char **exec_build_args(char *cmd, char **args)
{
    char **new_args = NULL;

    if (args == NULL)
        return exec_build_empty(cmd);
    new_args = exec_alloc_args(cmd, args);
    if (new_args == NULL)
        return NULL;
    for (int i = 0; args[i] != NULL; i++)
        new_args[i + 1] = args[i];
    free(args);
    return new_args;
}

char **exec_build_args_dup(char *cmd, char **args)
{
    char **new_args = NULL;
    char *dup_cmd = my_strdup(cmd);

    if (dup_cmd == NULL)
        return NULL;
    if (args == NULL)
        new_args = exec_build_empty(dup_cmd);
    else
        new_args = exec_alloc_args(dup_cmd, args);
    if (new_args == NULL) {
        free(dup_cmd);
        return NULL;
    }
    if (args != NULL) {
        for (int i = 0; args[i] != NULL; i++)
            new_args[i + 1] = args[i];
        free(args);
    }
    return new_args;
}
