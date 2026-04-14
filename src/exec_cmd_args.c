/*
** EPITECH PROJECT, 2026
** exec_cmd_args
** File description:
** exec_cmd_args
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

static int prepare_exec_args(char *cmd, char **args, char *path, char **env)
{
    char *full_cmd = exec_make_full_cmd(path, cmd);
    char **argv = NULL;

    if (full_cmd == NULL)
        return 84;
    if (exec_can_exec(full_cmd) != 0) {
        free(full_cmd);
        return 84;
    }
    argv = build_argv(full_cmd, args);
    free(full_cmd);
    if (argv == NULL)
        return 84;
    return my_execve(argv[0], argv, env);
}

static int exec_launch_format_arg_args(char *cmd, char **args, char **env)
{
    char **argv = build_argv(cmd, args);

    if (argv == NULL)
        return 84;
    if (exec_can_exec(cmd) != 0)
        return exec_free_args(argv);
    return my_execve(cmd, argv, env);
}

static int print_not_found(char *cmd)
{
    my_putstr_err(cmd);
    my_putstr_err(": Command not found.\n");
    return 1;
}

static int exec_path_loop(char *cmd, char **args, char **env,
    char **path_array_env)
{
    int exit_code = 84;

    for (int i = 0; path_array_env[i] != NULL && exit_code == 84; i++)
        exit_code = prepare_exec_args(cmd, args, path_array_env[i], env);
    return exit_code;
}

static int exec_path_env_args(char *cmd, char **args, char **env)
{
    char *path_env = my_getenv("PATH", env);
    char **path_array_env = NULL;
    int exit_code = 84;

    if (path_env == NULL) {
        my_putstr_err(cmd);
        my_putstr_err(": Command not found.\n");
        return 1;
    }
    path_array_env = my_str_to_word_array(path_env, ':');
    if (path_array_env == NULL)
        return 84;
    exit_code = exec_path_loop(cmd, args, env, path_array_env);
    if (exit_code == 84)
        exit_code = print_not_found(cmd);
    free_array(path_array_env);
    return exit_code;
}

int exec_cmd_args(char **args, char **env)
{
    if (args == NULL || args[0] == NULL || env == NULL)
        return 84;
    if (check_cmd(args[0]) != 0)
        return 1;
    if (args[0][0] == '/' || args[0][0] == '.')
        return exec_launch_format_arg_args(args[0], args, env);
    return exec_path_env_args(args[0], args, env);
}
