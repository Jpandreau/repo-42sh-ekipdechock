/*
** EPITECH PROJECT, 2026
** test_builtins_dispatch
** File description:
** Tests pour la reconnaissance et dispatch des builtins
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "base.h"
#include "buildin.h"
#include "my.h"
#include "small_headers.h"

Test(buildin, recognize_cd)
{
    cr_assert_eq(buildin("cd"), 1);
}

Test(buildin, recognize_env)
{
    cr_assert_eq(buildin("env"), 1);
}

Test(buildin, recognize_setenv)
{
    cr_assert_eq(buildin("setenv"), 1);
}

Test(buildin, recognize_unsetenv)
{
    cr_assert_eq(buildin("unsetenv"), 1);
}

Test(buildin, recognize_exit)
{
    cr_assert_eq(buildin("exit"), 1);
}

Test(buildin, recognize_history)
{
    cr_assert_eq(buildin("history"), 1);
}

Test(buildin, not_builtin_ls)
{
    cr_assert_eq(buildin("ls"), 0);
}

Test(buildin, not_builtin_gcc)
{
    cr_assert_eq(buildin("gcc"), 0);
}

Test(buildin, not_builtin_random)
{
    cr_assert_eq(buildin("random"), 0);
}

Test(buildin, null_input)
{
    cr_assert_eq(buildin(NULL), 0);
}

Test(buildin, empty_string)
{
    cr_assert_eq(buildin(""), 0);
}

Test(buildin, case_sensitive_CD)
{
    cr_assert_eq(buildin("CD"), 0);
}

Test(buildin, case_sensitive_ENV)
{
    cr_assert_eq(buildin("ENV"), 0);
}

Test(buildin, partial_match_cde)
{
    cr_assert_eq(buildin("cde"), 0);
}

Test(buildin, partial_match_set)
{
    cr_assert_eq(buildin("set"), 0);
}

Test(buildin, with_whitespace)
{
    cr_assert_eq(buildin("cd "), 0);
}

Test(my_getenv, find_path)
{
    char *env[] = {"PATH=/usr/bin", "HOME=/home/user", NULL};
    char *result = my_getenv("PATH", env);
    cr_assert_not_null(result);
    cr_assert_str_eq(result, "/usr/bin");
}

Test(my_getenv, find_home)
{
    char *env[] = {"PATH=/usr/bin", "HOME=/home/user", NULL};
    char *result = my_getenv("HOME", env);
    cr_assert_not_null(result);
    cr_assert_str_eq(result, "/home/user");
}

Test(my_getenv, not_found)
{
    char *env[] = {"PATH=/usr/bin", NULL};
    char *result = my_getenv("MISSING", env);
    cr_assert_null(result);
}

Test(my_getenv, empty_env)
{
    char *env[] = {NULL};
    char *result = my_getenv("PATH", env);
    cr_assert_null(result);
}

Test(my_getenv, first_var)
{
    char *env[] = {"TEST=value", "OTHER=val2", NULL};
    char *result = my_getenv("TEST", env);
    cr_assert_str_eq(result, "value");
}

Test(my_getenv, last_var)
{
    char *env[] = {"A=1", "B=2", "C=3", NULL};
    char *result = my_getenv("C", env);
    cr_assert_str_eq(result, "3");
}

Test(my_getenv, empty_value)
{
    char *env[] = {"EMPTY=", NULL};
    char *result = my_getenv("EMPTY", env);
    cr_assert_not_null(result);
    cr_assert_str_eq(result, "");
}

Test(my_getenv, value_with_equals)
{
    char *env[] = {"TEST=a=b=c", NULL};
    char *result = my_getenv("TEST", env);
    cr_assert_str_eq(result, "a=b=c");
}

Test(my_getenv, partial_name_match)
{
    char *env[] = {"PATHEXT=/bin", "PATH=/usr/bin", NULL};
    char *result = my_getenv("PATH", env);
    cr_assert_str_eq(result, "/usr/bin");
}

Test(run_buildin_args, null_args)
{
    char **env = NULL;

    cr_assert_eq(run_buildin_args(NULL, &env, NULL), 84);
}

Test(run_buildin_args, null_env)
{
    char *args[] = {"env", NULL};

    cr_assert_eq(run_buildin_args(args, NULL, NULL), 84);
}

Test(run_buildin_args, setenv_dispatch)
{
    char **env;
    char *args[] = {"setenv", "TEST", "value", NULL};
    int ret;

    env = malloc(sizeof(char *) * 1);
    env[0] = NULL;
    ret = run_buildin_args(args, &env, NULL);
    cr_assert_eq(ret, 0);
    cr_assert_not_null(env);
    cr_assert_str_eq(env[0], "TEST=value");
    free_array(env);
}

Test(run_buildin_args, unsetenv_dispatch)
{
    char **env;
    char *args[] = {"unsetenv", "TEST", NULL};
    int ret;

    env = malloc(sizeof(char *) * 2);
    env[0] = my_strdup("TEST=old");
    env[1] = NULL;
    ret = run_buildin_args(args, &env, NULL);
    cr_assert_eq(ret, 0);
    cr_assert_null(env[0]);
    free_array(env);
}

Test(run_buildin_args, history_dispatch_empty, .init = cr_redirect_stdout)
{
    char *args[] = {"history", NULL};
    char **env = NULL;
    history_t history = {0};

    history_init(&history);
    cr_assert_eq(run_buildin_args(args, &env, &history), 0);
    fflush(stdout);
    cr_assert_stdout_eq_str("");
    history_destroy(&history);
}

Test(run_buildin_args, history_dispatch_output, .init = cr_redirect_stdout)
{
    char *args[] = {"history", NULL};
    char **env = NULL;
    char expected[64] = {0};
    history_t history = {0};

    history_init(&history);
    history_add(&history, "ls");
    history_add(&history, "pwd");
    snprintf(expected, sizeof(expected), "1\tls\n2\tpwd\n");
    cr_assert_eq(run_buildin_args(args, &env, &history), 0);
    fflush(stdout);
    cr_assert_stdout_eq_str(expected);
    history_destroy(&history);
}

Test(run_buildin_args, history_with_args_returns_84)
{
    char *args[] = {"history", "10", NULL};
    char **env = NULL;

    cr_assert_eq(run_buildin_args(args, &env, NULL), 84);
}

Test(history_buildin_args, null_args_returns_84)
{
    cr_assert_eq(history_buildin_args(NULL, NULL), 84);
}

Test(history_buildin_args, null_argv0_returns_84)
{
    char *args[] = {NULL};

    cr_assert_eq(history_buildin_args(args, NULL), 84);
}

Test(history_buildin_args, prints_three_entries, .init = cr_redirect_stdout)
{
    char *args[] = {"history", NULL};
    char expected[96] = {0};
    history_t history = {0};

    history_init(&history);
    history_add(&history, "ls");
    history_add(&history, "pwd");
    history_add(&history, "echo ok");
    snprintf(expected, sizeof(expected), "1\tls\n2\tpwd\n3\techo ok\n");
    cr_assert_eq(history_buildin_args(args, &history), 0);
    fflush(stdout);
    cr_assert_stdout_eq_str(expected);
    history_destroy(&history);
}
