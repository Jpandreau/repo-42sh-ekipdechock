/*
** EPITECH PROJECT, 2026
** builtin_cd_path
** File description:
** cd path helpers
*/

#include "my.h"
#include "base.h"
#include "buildin.h"

static char *resolve_home_path(char *home)
{
    char *cwd = NULL;
    char *tmp = NULL;
    char *path = NULL;

    if (home == NULL || home[0] == '\0')
        return NULL;
    if (home[0] == '/')
        return my_strdup(home);
    cwd = getcwd(NULL, 0);
    if (cwd == NULL)
        return NULL;
    tmp = my_strcat(cwd, "/");
    free(cwd);
    if (tmp == NULL)
        return NULL;
    path = my_strcat(tmp, home);
    free(tmp);
    return path;
}

static char *get_home_path(char ***env)
{
    return resolve_home_path(my_getenv("HOME", *env));
}

int chdir_home(char ***env)
{
    char *path = get_home_path(env);
    int status = 0;

    if (path == NULL)
        return 84;
    status = my_chdir(path, env);
    free(path);
    return status;
}

char *expand_tilde_path(char *arg, char ***env)
{
    char *base = NULL;
    char *path = NULL;

    if (arg == NULL)
        return NULL;
    if (arg[0] != '~')
        return my_strdup(arg);
    if (arg[1] != '\0' && arg[1] != '/')
        return my_strdup(arg);
    base = get_home_path(env);
    if (base == NULL)
        return NULL;
    if (arg[1] == '\0')
        return base;
    path = my_strcat(base, arg + 1);
    free(base);
    return path;
}
