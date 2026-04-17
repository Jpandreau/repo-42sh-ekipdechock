/*
** EPITECH PROJECT, 2026
** my
** File description:
** my
*/

#ifndef MY
    #define MY
    #include <unistd.h>
    #include <sys/wait.h>
    #include <sys/stat.h>
    #include <fcntl.h>
    #include <stdio.h>
    #include <string.h>
    #include <stdlib.h>
    #include <signal.h>
    #include <errno.h>
    #include "small_headers.h"

void run(char *command, char **argument, char **env);
int exec_cmd(char *cmd, char *arg, char **env);
int check_cmd(char *cmd_path);
int exec_cmd_args(char **args, char **env);
int exec_cmd_args_nofork(char **args, char **env);
int exec_launch_format_arg(char *cmd, char *arg, char **env);
int exec_path_env(char *cmd, char *arg, char **env);
char *exec_make_full_cmd(char *path, char *cmd);
char **exec_build_args(char *cmd, char **args);
char **exec_build_args_dup(char *cmd, char **args);
int exec_can_exec(char *full_cmd);
int exec_free_cmd(char *cmd);
int exec_free_args(char **args);
int exec_free_args_cmd(char **args, char *cmd);
int my_execve(char *cmd, char *args[], char **env);
int prepare_exec(char *cmd, char *arg, char *path, char **env);

int buildin(char *line);
int actions_cmd_args(char **args, char ***env, history_t *history);
int actions_cmd_args_nofork(char **args, char ***env, history_t *history);
int script_loop(char **env);

int clean_getline(char **line, size_t *len);

void handle_sigint(int sig);

#endif /* !MY */
