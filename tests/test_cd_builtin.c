/*
** EPITECH PROJECT, 2026
** test_cd_builtin
** File description:
** Tests pour cd builtin
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <stdlib.h>
#include <unistd.h>
#include "buildin.h"
#include "base.h"

Test(cd_buildin_args, no_args_no_home)
{
    char **env = NULL;
    char *args[] = {"cd", NULL};

    env = malloc(sizeof(char *) * 1);
    env[0] = NULL;
    cr_assert_eq(cd_buildin_args(args, &env), 84);
    free(env);
}

Test(cd_buildin_args, too_many_args)
{
    char **env = NULL;
    char *args[] = {"cd", "/tmp", "/var", NULL};

    env = malloc(sizeof(char *) * 1);
    env[0] = NULL;
    cr_assert_eq(cd_buildin_args(args, &env), 84);
    free(env);
}

Test(cd_buildin_args, dash_no_oldpwd)
{
    char **env;
    char *args[] = {"cd", "-", NULL};

    env = malloc(sizeof(char *) * 1);
    env[0] = NULL;
    cr_assert_eq(cd_buildin_args(args, &env), 84);
    free(env);
}

Test(cd_buildin_args, nonexistent_dir)
{
    char **env;
    char *args[] = {"cd", "/no/such/directory", NULL};

    env = malloc(sizeof(char *) * 1);
    env[0] = NULL;
    cr_assert_eq(cd_buildin_args(args, &env), 84);
    free(env);
}

Test(cd_buildin_args, tilde_no_home)
{
    char **env;
    char *args[] = {"cd", "~", NULL};

    env = malloc(sizeof(char *) * 1);
    env[0] = NULL;
    cr_assert_eq(cd_buildin_args(args, &env), 84);
    free(env);
}
