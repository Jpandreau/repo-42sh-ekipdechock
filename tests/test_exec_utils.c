/*
** EPITECH PROJECT, 2026
** test_exec_utils
** File description:
** Tests pour les utilitaires exec
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include "base.h"
#include "my.h"

Test(exec_build_args, args_null)
{
    char *cmd = my_strdup("ls");
    char **result = exec_build_args(cmd, NULL);
    cr_assert_not_null(result);
    cr_assert_str_eq(result[0], "ls");
    cr_assert_null(result[1]);
    free_array(result);
}

Test(exec_build_args, with_args)
{
    char **args;
    char *cmd;
    char **result;

    args = malloc(sizeof(char *) * 3);
    args[0] = my_strdup("-l");
    args[1] = my_strdup("/tmp");
    args[2] = NULL;
    cmd = my_strdup("ls");
    result = exec_build_args(cmd, args);
    cr_assert_not_null(result);
    cr_assert_str_eq(result[0], "ls");
    cr_assert_str_eq(result[1], "-l");
    cr_assert_str_eq(result[2], "/tmp");
    cr_assert_null(result[3]);
    free_array(result);
}

Test(exec_build_args_dup, args_null)
{
    char **result = exec_build_args_dup("ls", NULL);
    cr_assert_not_null(result);
    cr_assert_str_eq(result[0], "ls");
    cr_assert_null(result[1]);
    free_array(result);
}

Test(exec_build_args_dup, with_args)
{
    char **args;
    char **result;

    args = malloc(sizeof(char *) * 3);
    args[0] = my_strdup("-a");
    args[1] = my_strdup(".");
    args[2] = NULL;
    result = exec_build_args_dup("ls", args);
    cr_assert_not_null(result);
    cr_assert_str_eq(result[0], "ls");
    cr_assert_str_eq(result[1], "-a");
    cr_assert_str_eq(result[2], ".");
    cr_assert_null(result[3]);
    free_array(result);
}

Test(exec_make_full_cmd, simple_join)
{
    char *full = exec_make_full_cmd("/bin", "ls");
    cr_assert_not_null(full);
    cr_assert_str_eq(full, "/bin/ls");
    free(full);
}

Test(exec_can_exec, non_existing)
{
    cr_assert_eq(exec_can_exec("/no/such/file"), 84);
}

Test(exec_can_exec, existing_and_executable)
{
    char template[] = "/tmp/minishell_exec_XXXXXX";
    int fd;
    int status;

    fd = mkstemp(template);
    cr_assert_geq(fd, 0);
    close(fd);
    chmod(template, 0755);
    status = exec_can_exec(template);
    unlink(template);
    cr_assert_eq(status, 0);
}

Test(exec_free_cmd, returns_84)
{
    char *cmd = my_strdup("ls");
    cr_assert_eq(exec_free_cmd(cmd), 84);
}

Test(exec_free_args, returns_84)
{
    char **args;

    args = malloc(sizeof(char *) * 2);
    args[0] = my_strdup("ls");
    args[1] = NULL;
    cr_assert_eq(exec_free_args(args), 84);
}

Test(exec_free_args_cmd, returns_84)
{
    char **args;
    char *cmd;

    args = malloc(sizeof(char *) * 2);
    args[0] = my_strdup("ls");
    args[1] = NULL;
    cmd = my_strdup("cmd");
    cr_assert_eq(exec_free_args_cmd(args, cmd), 84);
}
