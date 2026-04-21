/*
** EPITECH PROJECT, 2026
** buildin
** File description:
** buildin
*/

#ifndef BUILDIN
    #define BUILDIN

    #include "small_headers.h"

int my_chdir(char *path, char ***env);
int chdir_home(char ***env);
char *expand_tilde_path(char *arg, char ***env);

typedef struct job_state_s job_state_t;

int run_buildin_args(char **args, char ***env, history_t *history,
    job_state_t *job);
int cd_buildin_args(char **args, char ***env);
int env_buildin_args(char **args, char **env);
int history_buildin_args(char **args, history_t *history);
int setenv_buildin_args(char **args, char ***env);
int unsetenv_buildin_args(char **args, char ***env);
int exit_buildin_args(char **args);
int fg_buildin_args(char **args, job_state_t *job);
int bg_buildin_args(char **args, job_state_t *job);

char *my_getenv(char *key, char **env);

#endif /* !BUILDIN */
