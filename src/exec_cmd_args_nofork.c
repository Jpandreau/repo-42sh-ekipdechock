/*
** EPITECH PROJECT, 2026
** exec_cmd_args_nofork
** File description:
** exec_cmd_args_nofork
*/

#include "my.h"
#include "base.h"
#include "buildin.h"

static char **alloc_argv(int size)
{
    char **argv = malloc(sizeof(char *) * (size + 1));

    if (argv == NULL)
        return NULL;
    for (int i = 0; i <= size; i++)
        argv[i] = NULL;
    return argv;
}

static char **build_argv(char *cmd, char **args)
{
    int size = array_size(args);
    char **argv = alloc_argv(size);

    if (argv == NULL)
        return NULL;
    argv[0] = my_strdup(cmd);
    if (argv[0] == NULL)
        return exec_free_args(argv), NULL;
    for (int i = 1; i < size; i++) {
        argv[i] = my_strdup(args[i]);
        if (argv[i] == NULL)
            return exec_free_args(argv), NULL;
    }
    return argv;
}

static int print_not_found(char *cmd)
{
    my_putstr_err(cmd);
    my_putstr_err(": Command not found.\n");
    return 0;
}

static int exec_launch_format_arg_args_nofork(char *cmd, char **args,
    char **env)
{
    char **argv = build_argv(cmd, args);

    if (argv == NULL)
        return 84;
    if (exec_can_exec(cmd) != 0)
        return exec_free_args(argv);
    execve(cmd, argv, env);
    exec_free_args(argv);
    return 84;
}

static int try_exec_path(char *cmd, char **args, char **env, char *path)
{
    char *full_cmd = exec_make_full_cmd(path, cmd);

    if (full_cmd == NULL)
        return 0;
    if (exec_can_exec(full_cmd) == 0) {
        exec_launch_format_arg_args_nofork(full_cmd, args, env);
        free(full_cmd);
        return 1;
    }
    free(full_cmd);
    return 0;
}

static int exec_path_env_args_nofork(char *cmd, char **args, char **env)
{
    char *path_env = my_getenv("PATH", env);
    char **path_array_env = NULL;
    int found = 0;

    if (path_env == NULL) {
        my_putstr_err(cmd);
        my_putstr_err(": Command not found.\n");
        return 84;
    }
    path_array_env = my_str_to_word_array(path_env, ':');
    if (path_array_env == NULL)
        return 84;
    for (int i = 0; path_array_env[i] != NULL; i++)
        found |= try_exec_path(cmd, args, env, path_array_env[i]);
    if (!found) {
        free_array(path_array_env);
        return print_not_found(cmd);
    }
    free_array(path_array_env);
    return 0;
}

int exec_cmd_args_nofork(char **args, char **env)
{
    if (args == NULL || args[0] == NULL || env == NULL)
        return 84;
    if (check_cmd(args[0]) != 0)
        return 1;
    if (args[0][0] == '/' || args[0][0] == '.' || strchr(args[0], '/'))
        return exec_launch_format_arg_args_nofork(args[0], args, env);
    return exec_path_env_args_nofork(args[0], args, env);
}
