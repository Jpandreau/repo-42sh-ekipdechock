/*
** EPITECH PROJECT, 2026
** test_exec_cmd_runtime
** File description:
** runtime tests for exec_cmd and my_execve helpers
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>
#include "my.h"
#include "base.h"

int display_segfault(int status);

static char **make_args_one(char *a0)
{
    char **args = malloc(sizeof(char *) * 2);

    if (args == NULL)
        return NULL;
    args[0] = my_strdup(a0);
    args[1] = NULL;
    return args;
}

Test(exec_cmd_runtime, display_segfault_non_signal)
{
    cr_assert_eq(display_segfault(0), 0);
}

Test(exec_cmd_runtime, display_segfault_signal_case)
{
    pid_t pid = fork();
    int status = 0;

    cr_assert_neq(pid, -1);
    if (pid == 0) {
        raise(SIGSEGV);
        _exit(0);
    }
    waitpid(pid, &status, 0);
    cr_assert_eq(display_segfault(status), 139);
}

Test(exec_cmd_runtime, my_execve_true_returns_0)
{
    char **args = make_args_one("/bin/true");
    char *env[] = {"PATH=/bin:/usr/bin", NULL};

    cr_assert_not_null(args);
    cr_assert_eq(my_execve("/bin/true", args, env), 0);
}

Test(exec_cmd_runtime, my_execve_missing_binary_returns_1)
{
    char **args = make_args_one("/no/such/cmd");
    char *env[] = {"PATH=/bin:/usr/bin", NULL};

    cr_assert_not_null(args);
    cr_assert_eq(my_execve("/no/such/cmd", args, env), 1);
}

Test(exec_cmd_runtime, exec_cmd_absolute_success)
{
    char *env[] = {"PATH=/bin:/usr/bin", NULL};

    cr_assert_eq(exec_cmd("/bin/true", NULL, env), 0);
}

Test(exec_cmd_runtime, exec_cmd_absolute_missing_returns_1,
    .init = cr_redirect_stderr)
{
    char *env[] = {"PATH=/bin:/usr/bin", NULL};

    cr_assert_eq(exec_cmd("/no/such/cmd", NULL, env), 1);
    fflush(stderr);
    cr_assert_stderr_eq_str("/no/such/cmd: Command not found.\n");
}

Test(exec_cmd_runtime, exec_launch_format_arg_success_paths)
{
    char *env[] = {"PATH=/bin:/usr/bin", NULL};

    cr_assert_eq(exec_launch_format_arg("/bin/true", NULL, env), 0);
    cr_assert_eq(exec_launch_format_arg("/bin/echo", "ok", env), 0);
}
