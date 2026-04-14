/*
** EPITECH PROJECT, 2026
** builtin_cd
** File description:
** cd builtin helpers
*/

#include "my.h"
#include "base.h"
#include "buildin.h"

static int setenv_helper(char *key, char *value, char ***env)
{
    char *args[4];

    args[0] = "setenv";
    args[1] = key;
    args[2] = value;
    args[3] = NULL;
    return setenv_buildin_args(args, env);
}

static int set_oldpwd(char ***env)
{
    char *pwd = my_getenv("PWD", *env);

    if (pwd == NULL || pwd[0] == '\0')
        return 84;
    return setenv_helper("OLDPWD", pwd, env);
}

static int set_newpwd(char *new_pwd, char ***env)
{
    if (new_pwd == NULL || new_pwd[0] == '\0')
        return 84;
    return setenv_helper("PWD", new_pwd, env);
}

static int update_pwd_after_chdir(char ***env)
{
    char *real_path = getcwd(NULL, 0);
    int status = 0;

    if (real_path == NULL)
        return 84;
    status = set_newpwd(real_path, env);
    free(real_path);
    return status;
}

static int is_a_dir(char *path)
{
    struct stat st;

    if (stat(path, &st) != 0)
        return 0;
    return S_ISDIR(st.st_mode);
}

int my_chdir(char *path, char ***env)
{
    if (access(path, F_OK) == -1) {
        my_putstr_err(path);
        my_putstr_err(": No such file or directory.\n");
        return 84;
    }
    if (access(path, R_OK) == -1) {
        my_putstr_err(path);
        my_putstr_err(": Permission denied.\n");
        return 84;
    }
    if (!is_a_dir(path)) {
        my_putstr_err(path);
        my_putstr_err(": Not a directory.\n");
        return 84;
    }
    if (set_oldpwd(env) != 0)
        return 84;
    if (chdir(path) != 0)
        return 84;
    return update_pwd_after_chdir(env);
}

static int change_oldpwd_args(char ***env)
{
    char *oldpwd = my_getenv("OLDPWD", *env);
    char *dup = NULL;
    int status = 0;

    if (oldpwd == NULL)
        return 84;
    dup = my_strdup(oldpwd);
    if (dup == NULL)
        return 84;
    status = my_chdir(dup, env);
    free(dup);
    return status;
}

int cd_buildin_args(char **args, char ***env)
{
    int size = array_size(args);
    char *path = NULL;
    int status = 0;

    if (size == 1)
        return chdir_home(env);
    if (size > 2)
        return 84;
    if (my_strcmp(args[1], "-") == 0)
        return change_oldpwd_args(env);
    path = expand_tilde_path(args[1], env);
    if (path == NULL)
        return 84;
    status = my_chdir(path, env);
    free(path);
    return status;
}
