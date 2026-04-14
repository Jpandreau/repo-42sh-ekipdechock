/*
** EPITECH PROJECT, 2026
** exec_manage_path
** File description:
** exec_manage_path
*/

#include "my.h"
#include "base.h"

char *exec_make_full_cmd(char *path, char *cmd)
{
    char *full_path = my_strcat(path, "/");
    char *full_cmd = NULL;

    if (full_path == NULL)
        return NULL;
    full_cmd = my_strcat(full_path, cmd);
    free(full_path);
    return full_cmd;
}

int exec_can_exec(char *full_cmd)
{
    if (access(full_cmd, F_OK) == -1)
        return 84;
    if (access(full_cmd, X_OK) == -1)
        return 84;
    return 0;
}
