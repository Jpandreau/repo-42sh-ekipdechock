/*
** EPITECH PROJECT, 2026
** test_exit_builtin
** File description:
** Tests pour exit_buildin_args
*/

#include <criterion/criterion.h>
#include <sys/wait.h>
#include <unistd.h>
#include "buildin.h"

Test(exit_buildin_args, exit_zero)
{
    pid_t pid = fork();

    if (pid == 0) {
        char *args[] = {"exit", NULL};
        exit_buildin_args(args);
        _exit(1);
    }
    cr_assert_gt(pid, 0);
    int status = 0;
    waitpid(pid, &status, 0);
    cr_assert(WIFEXITED(status));
    cr_assert_eq(WEXITSTATUS(status), 0);
}

Test(exit_buildin_args, exit_code)
{
    pid_t pid = fork();

    if (pid == 0) {
        char *args[] = {"exit", "42", NULL};
        exit_buildin_args(args);
        _exit(1);
    }
    cr_assert_gt(pid, 0);
    int status = 0;
    waitpid(pid, &status, 0);
    cr_assert(WIFEXITED(status));
    cr_assert_eq(WEXITSTATUS(status), 42);
}

Test(exit_buildin_args, too_many_args)
{
    char *args[] = {"exit", "1", "2", NULL};

    cr_assert_eq(exit_buildin_args(args), 84);
}

Test(exit_buildin_args, invalid_arg)
{
    char *args[] = {"exit", "abc", NULL};

    cr_assert_eq(exit_buildin_args(args), 84);
}
