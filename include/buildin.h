/*
** EPITECH PROJECT, 2026
** buildin
** File description:
** buildin
*/

#ifndef BUILDIN
    #define BUILDIN

int my_chdir(char *path, char ***env);
int chdir_home(char ***env);
char *expand_tilde_path(char *arg, char ***env);

int run_buildin_args(char **args, char ***env);
int cd_buildin_args(char **args, char ***env);
int env_buildin_args(char **args, char **env);
int setenv_buildin_args(char **args, char ***env);
int unsetenv_buildin_args(char **args, char ***env);
int exit_buildin_args(char **args);

char *my_getenv(char *key, char **env);

#endif /* !BUILDIN */
