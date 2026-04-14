/*
** EPITECH PROJECT, 2026
** test_exec_path_exit_status_sigint
** File description:
** extra coverage for exec path helpers and exit status helpers
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "my.h"
#include "tree.h"

Test(exec_launch_format_arg, missing_executable_with_args)
{
    char *env[] = {"PATH=/bin:/usr/bin", NULL};

    cr_assert_eq(exec_launch_format_arg("/no/such/cmd", "hello world", env), 84);
}

Test(exec_launch_format_arg, missing_executable_without_args)
{
    char *env[] = {"PATH=/bin:/usr/bin", NULL};

    cr_assert_eq(exec_launch_format_arg("/no/such/cmd", NULL, env), 84);
}

Test(exec_path_env, missing_path_returns_84)
{
    char *env[] = {"HOME=/tmp", NULL};

    cr_assert_eq(exec_path_env("ls", NULL, env), 84);
}

Test(exec_path_env, command_not_found_prints_english_message,
    .init = cr_redirect_stderr)
{
    char *env[] = {"PATH=/definitely_not_here_1:/definitely_not_here_2", NULL};

    cr_assert_eq(exec_path_env("not_a_real_cmd", NULL, env), 0);
    fflush(stderr);
    cr_assert_stderr_eq_str("not_a_real_cmd: Command not found.\n");
}

Test(exit_status_helpers, round_trip_and_bounds)
{
    cr_assert_eq(make_exit_status(0), -1000);
    cr_assert_eq(make_exit_status(42), -1042);
    cr_assert_eq(exit_status_code(-1000), 0);
    cr_assert_eq(exit_status_code(-1042), 42);
    cr_assert_eq(is_exit_status(-1000), 1);
    cr_assert_eq(is_exit_status(-1255), 1);
    cr_assert_eq(is_exit_status(-999), 0);
    cr_assert_eq(is_exit_status(-1256), 0);
}

Test(handle_sigint, prints_prompt, .init = cr_redirect_stdout)
{
    handle_sigint(SIGINT);
    fflush(stdout);
    cr_assert_stdout_eq_str("\n$> ");
}
