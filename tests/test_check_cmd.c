/*
** EPITECH PROJECT, 2026
** test_check_cmd
** File description:
** Tests pour check_cmd et exec_can_exec
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include "my.h"

Test(check_cmd, null_cmd)
{
    cr_assert_eq(check_cmd(NULL), 84);
}

Test(check_cmd, absolute_path_missing, .init = cr_redirect_stderr)
{
    cr_assert_eq(check_cmd("/no/such/cmd"), 84);
    fflush(stderr);
    cr_assert_stderr_eq_str("/no/such/cmd: Command not found.\n");
}

Test(check_cmd, dot_path_missing, .init = cr_redirect_stderr)
{
    cr_assert_eq(check_cmd("./no/such/cmd"), 84);
    fflush(stderr);
    cr_assert_stderr_eq_str("./no/such/cmd: Command not found.\n");
}

Test(check_cmd, absolute_path_not_executable, .init = cr_redirect_stderr)
{
    char template[] = "/tmp/minishell_noexec_XXXXXX";
    int fd;

    fd = mkstemp(template);
    cr_assert_geq(fd, 0);
    close(fd);
    chmod(template, 0644);
    cr_assert_eq(check_cmd(template), 84);
    fflush(stderr);
    unlink(template);
}

Test(check_cmd, absolute_path_executable)
{
    char template[] = "/tmp/minishell_exec_XXXXXX";
    int fd;

    fd = mkstemp(template);
    cr_assert_geq(fd, 0);
    close(fd);
    chmod(template, 0755);
    cr_assert_eq(check_cmd(template), 0);
    unlink(template);
}

Test(check_cmd, regular_cmd)
{
    cr_assert_eq(check_cmd("ls"), 0);
}

Test(check_cmd, builtin_cmd)
{
    cr_assert_eq(check_cmd("env"), 0);
}
