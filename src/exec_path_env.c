/*
** EPITECH PROJECT, 2026
** exec_path_env
** File description:
** exec_path_env
*/

#include "my.h"
#include "base.h"
#include "buildin.h"

int exec_path_env(char *cmd, char *arg, char **env)
{
    char *path_env = my_getenv("PATH", env);
    char **path_array_env = NULL;
    int exit_code = 84;

    if (path_env == NULL)
        return 84;
    path_array_env = my_str_to_word_array(path_env, ':');
    if (path_array_env == NULL)
        return 84;
    for (int i = 0; path_array_env[i] != NULL && exit_code == 84; i++)
        exit_code = prepare_exec(cmd, arg, path_array_env[i], env);
    if (exit_code == 84){
        my_putstr_err(cmd);
        my_putstr_err(": Command not found.\n");
        exit_code = 0;
    }
    free_array(path_array_env);
    return exit_code;
}
