/*
** EPITECH PROJECT, 2026
** test_env_operations
** File description:
** Tests pour les opérations sur l'environnement (setenv/unsetenv)
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "base.h"
#include "buildin.h"
#include "my.h"

Test(env_buildin_args, display_empty_env, .init = cr_redirect_stdout)
{
    char *env[] = {NULL};
    char *args[] = {"env", NULL};
    int ret;

    ret = env_buildin_args(args, env);
    cr_assert_eq(ret, 0);
}

Test(env_buildin_args, display_single_var, .init = cr_redirect_stdout)
{
    char *env[] = {"PATH=/bin", NULL};
    char *args[] = {"env", NULL};
    int ret;

    ret = env_buildin_args(args, env);
    cr_assert_eq(ret, 0);
    fflush(stdout);
    cr_assert_stdout_eq_str("PATH=/bin\n");
}

Test(env_buildin_args, display_multiple_vars, .init = cr_redirect_stdout)
{
    char *env[] = {"A=1", "B=2", "C=3", NULL};
    char *args[] = {"env", NULL};
    int ret;

    ret = env_buildin_args(args, env);
    cr_assert_eq(ret, 0);
    fflush(stdout);
    cr_assert_stdout_eq_str("A=1\nB=2\nC=3\n");
}
Test(setenv_buildin_args, set_new_var)
{
    char *args[] = {"setenv", "TEST", "value", NULL};
    char **env;
    int ret;

    env = malloc(sizeof(char *) * 1);
    env[0] = NULL;
    ret = setenv_buildin_args(args, &env);
    cr_assert_eq(ret, 0);
    cr_assert_not_null(env);
    cr_assert_str_eq(env[0], "TEST=value");
    cr_assert_null(env[1]);
    free_array(env);
}

Test(setenv_buildin_args, set_key_value_with_equal)
{
    char *args[] = {"setenv", "TEST=value", NULL};
    char **env;
    int ret;

    env = malloc(sizeof(char *) * 1);
    env[0] = NULL;
    ret = setenv_buildin_args(args, &env);
    cr_assert_eq(ret, 0);
    cr_assert_not_null(env);
    cr_assert_str_eq(env[0], "TEST=value");
    cr_assert_null(env[1]);
    free_array(env);
}

Test(setenv_buildin_args, set_key_value_with_equal_in_value)
{
    char *args[] = {"setenv", "TEST=a=b", NULL};
    char **env;
    int ret;

    env = malloc(sizeof(char *) * 1);
    env[0] = NULL;
    ret = setenv_buildin_args(args, &env);
    cr_assert_eq(ret, 0);
    cr_assert_not_null(env);
    cr_assert_str_eq(env[0], "TEST=a=b");
    cr_assert_null(env[1]);
    free_array(env);
}

Test(setenv_buildin_args, set_existing_var)
{
    char *args[] = {"setenv", "TEST", "new", NULL};
    char **env;
    int ret;

    env = malloc(sizeof(char *) * 2);
    env[0] = my_strdup("TEST=old");
    env[1] = NULL;
    ret = setenv_buildin_args(args, &env);
    cr_assert_eq(ret, 0);
    cr_assert_str_eq(env[0], "TEST=new");
    cr_assert_null(env[1]);
    free_array(env);
}

Test(setenv_buildin_args, missing_args, .init = cr_redirect_stdout)
{
    char *args[] = {"setenv", NULL};
    char **env;
    int ret;

    env = malloc(sizeof(char *) * 2);
    env[0] = my_strdup("PATH=/bin");
    env[1] = NULL;
    ret = setenv_buildin_args(args, &env);
    cr_assert_eq(ret, 0);
    fflush(stdout);
    cr_assert_stdout_eq_str("PATH=/bin\n");
    free_array(env);
}

Test(setenv_buildin_args, missing_value, .init = cr_redirect_stderr)
{
    char *args[] = {"setenv", "TEST", NULL};
    char **env;
    int ret;

    env = malloc(sizeof(char *) * 1);
    env[0] = NULL;
    ret = setenv_buildin_args(args, &env);
    cr_assert_eq(ret, 0);
    cr_assert_str_eq(env[0], "TEST=");
    free_array(env);
}

Test(setenv_buildin_args, set_path)
{
    char *args[] = {"setenv", "PATH", "/usr/bin:/bin", NULL};
    char **env;
    int ret;

    env = malloc(sizeof(char *) * 1);
    env[0] = NULL;
    ret = setenv_buildin_args(args, &env);
    cr_assert_eq(ret, 0);
    cr_assert_str_eq(env[0], "PATH=/usr/bin:/bin");
    free_array(env);
}

Test(setenv_buildin_args, set_empty_value)
{
    char *args[] = {"setenv", "EMPTY", "", NULL};
    char **env;
    int ret;

    env = malloc(sizeof(char *) * 1);
    env[0] = NULL;
    ret = setenv_buildin_args(args, &env);
    cr_assert_eq(ret, 0);
    cr_assert_str_eq(env[0], "EMPTY=");
    free_array(env);
}

Test(setenv_buildin_args, overwrite_middle)
{
    char *args[] = {"setenv", "B", "new", NULL};
    char **env;
    int ret;

    env = malloc(sizeof(char *) * 4);
    env[0] = my_strdup("A=1");
    env[1] = my_strdup("B=2");
    env[2] = my_strdup("C=3");
    env[3] = NULL;
    ret = setenv_buildin_args(args, &env);
    cr_assert_eq(ret, 0);
    cr_assert_str_eq(env[0], "A=1");
    cr_assert_str_eq(env[1], "B=new");
    cr_assert_str_eq(env[2], "C=3");
    free_array(env);
}

Test(unsetenv_buildin_args, unset_existing)
{
    char *args[] = {"unsetenv", "PATH", NULL};
    char **env;
    int ret;

    env = malloc(sizeof(char *) * 3);
    env[0] = my_strdup("PATH=/bin");
    env[1] = my_strdup("HOME=/home");
    env[2] = NULL;
    ret = unsetenv_buildin_args(args, &env);
    cr_assert_eq(ret, 0);
    cr_assert_str_eq(env[0], "HOME=/home");
    cr_assert_null(env[1]);
    free_array(env);
}

Test(unsetenv_buildin_args, unset_nonexistent)
{
    char *args[] = {"unsetenv", "MISSING", NULL};
    char **env;
    int ret;

    env = malloc(sizeof(char *) * 2);
    env[0] = my_strdup("PATH=/bin");
    env[1] = NULL;
    ret = unsetenv_buildin_args(args, &env);
    cr_assert_eq(ret, 0);
    cr_assert_str_eq(env[0], "PATH=/bin");
    free_array(env);
}

Test(unsetenv_buildin_args, missing_args, .init = cr_redirect_stderr)
{
    char *args[] = {"unsetenv", NULL};
    char **env;
    int ret;

    env = malloc(sizeof(char *) * 1);
    env[0] = NULL;
    ret = unsetenv_buildin_args(args, &env);
    cr_assert_eq(ret, 84);
    free_array(env);
}

Test(unsetenv_buildin_args, unset_multiple)
{
    char *args[] = {"unsetenv", "A", "C", NULL};
    char **env;
    int ret;

    env = malloc(sizeof(char *) * 4);
    env[0] = my_strdup("A=1");
    env[1] = my_strdup("B=2");
    env[2] = my_strdup("C=3");
    env[3] = NULL;
    ret = unsetenv_buildin_args(args, &env);
    cr_assert_eq(ret, 0);
    cr_assert_str_eq(env[0], "B=2");
    cr_assert_null(env[1]);
    free_array(env);
}

Test(unsetenv_buildin_args, unset_all)
{
    char *args[] = {"unsetenv", "A", "B", NULL};
    char **env;
    int ret;

    env = malloc(sizeof(char *) * 3);
    env[0] = my_strdup("A=1");
    env[1] = my_strdup("B=2");
    env[2] = NULL;
    ret = unsetenv_buildin_args(args, &env);
    cr_assert_eq(ret, 0);
    cr_assert_null(env[0]);
    free_array(env);
}

Test(unsetenv_buildin_args, unset_first)
{
    char *args[] = {"unsetenv", "A", NULL};
    char **env;
    int ret;

    env = malloc(sizeof(char *) * 4);
    env[0] = my_strdup("A=1");
    env[1] = my_strdup("B=2");
    env[2] = my_strdup("C=3");
    env[3] = NULL;
    ret = unsetenv_buildin_args(args, &env);
    cr_assert_eq(ret, 0);
    cr_assert_str_eq(env[0], "B=2");
    cr_assert_str_eq(env[1], "C=3");
    cr_assert_null(env[2]);
    free_array(env);
}

Test(unsetenv_buildin_args, unset_last)
{
    char *args[] = {"unsetenv", "C", NULL};
    char **env;
    int ret;

    env = malloc(sizeof(char *) * 4);
    env[0] = my_strdup("A=1");
    env[1] = my_strdup("B=2");
    env[2] = my_strdup("C=3");
    env[3] = NULL;
    ret = unsetenv_buildin_args(args, &env);
    cr_assert_eq(ret, 0);
    cr_assert_str_eq(env[0], "A=1");
    cr_assert_str_eq(env[1], "B=2");
    cr_assert_null(env[2]);
    free_array(env);
}
