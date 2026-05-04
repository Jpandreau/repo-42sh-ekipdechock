/*
** EPITECH PROJECT, 2026
** exec_cmd
** File description:
** exec_cmd
*/

#include "my.h"
#include "base.h"
#include "buildin.h"

static int cmd_is_path(char *cmd)
{
    int i = 0;

    if (cmd[0] == '/' || cmd[0] == '.')
        return 1;
    for (i = 0; cmd[i] != '\0'; i++)
        if (cmd[i] == '/')
            return 1;
    return 0;
}

int check_cmd(char *cmd_path)
{
    if (cmd_path == NULL)
        return 84;
    if (!cmd_is_path(cmd_path))
        return 0;
    if (access(cmd_path, F_OK) == -1) {
        my_putstr_err(cmd_path);
        my_putstr_err(": Command not found.\n");
        return 84;
    }
    if (access(cmd_path, X_OK) == -1) {
        my_putstr_err(cmd_path);
        my_putstr_err(": Permission denied.\n");
        return 84;
    }
    return 0;
}

int display_segfault(int status)
{
    if (WIFSIGNALED(status) && WTERMSIG(status) == SIGSEGV) {
        my_putstr_err("Segmentation fault");
        if (WCOREDUMP(status))
            my_putstr_err(" (core dumped)");
        my_putstr_err("\n");
        return 139;
    }
    return 0;
}

int my_execve(char *cmd, char *args[], char **env)
{
    pid_t pid = 0;
    int status = 0;

    pid = fork();
    if (pid == -1) {
        free_array(args);
        return 84;
    }
    if (pid == 0) {
        execve(cmd, args, env);
        exit(1);
    } else {
        waitpid(pid, &status, 0);
        free_array(args);
        if (display_segfault(status) != 0)
            return 139;
        return WEXITSTATUS(status);
    }
}

int prepare_exec(char *cmd, char *arg, char *path, char **env)
{
    char *full_cmd = exec_make_full_cmd(path, cmd);
    char **arg_array = my_str_to_word_array(arg, ' ');
    char **final_args = NULL;

    if (full_cmd == NULL)
        return 84;
    if (arg != NULL && arg_array == NULL)
        return exec_free_cmd(full_cmd);
    final_args = exec_build_args(full_cmd, arg_array);
    if (final_args == NULL)
        return exec_free_args_cmd(arg_array, full_cmd);
    if (exec_can_exec(full_cmd) != 0)
        return exec_free_args(final_args);
    return my_execve(full_cmd, final_args, env);
}

int exec_cmd(char *cmd, char *arg, char **env)
{
    if (cmd == NULL || env == NULL)
        return 84;
    if (check_cmd(cmd) != 0)
        return 1;
    if (cmd_is_path(cmd))
        return exec_launch_format_arg(cmd, arg, env);
    return exec_path_env(cmd, arg, env);
}
