/*
** EPITECH PROJECT, 2026
** test_actions_cmd
** File description:
** Tests pour actions_cmd
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "my.h"

Test(actions_cmd_args, null_args)
{
    char **env = NULL;

    cr_assert_eq(actions_cmd_args(NULL, &env, NULL), 84);
}

Test(actions_cmd_args, null_command)
{
    char **env = NULL;
    char *args[] = {NULL};

    cr_assert_eq(actions_cmd_args(args, &env, NULL), 84);
}

Test(actions_cmd_args_nofork, null_args)
{
    char **env = NULL;

    cr_assert_eq(actions_cmd_args_nofork(NULL, &env, NULL), 84);
}

Test(actions_cmd_args_nofork, null_command)
{
    char **env = NULL;
    char *args[] = {NULL};

    cr_assert_eq(actions_cmd_args_nofork(args, &env, NULL), 84);
}

Test(actions_cmd_args, builtin_env, .init = cr_redirect_stdout)
{
    char *env[] = {"A=1", NULL};
    char **env_ptr = env;
    char *args[] = {"env", NULL};
    int ret;

    ret = actions_cmd_args(args, &env_ptr, NULL);
    cr_assert_eq(ret, 0);
    fflush(stdout);
    cr_assert_stdout_eq_str("A=1\n");
}

Test(actions_cmd_args_nofork, builtin_env, .init = cr_redirect_stdout)
{
    char *env[] = {"B=2", NULL};
    char **env_ptr = env;
    char *args[] = {"env", NULL};
    int ret;

    ret = actions_cmd_args_nofork(args, &env_ptr, NULL);
    cr_assert_eq(ret, 0);
    fflush(stdout);
    cr_assert_stdout_eq_str("B=2\n");
}
