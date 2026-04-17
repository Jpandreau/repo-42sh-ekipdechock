/*
** EPITECH PROJECT, 2026
** test_cd_and_exec_args_more
** File description:
** extra coverage for cd and exec_cmd_args variants
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "buildin.h"
#include "my.h"
#include "base.h"

static char **make_env_pwd(char *pwd)
{
    char **env = malloc(sizeof(char *) * 3);

    if (env == NULL)
        return NULL;
    env[0] = my_strdup(pwd);
    env[1] = my_strdup("HOME=/tmp");
    env[2] = NULL;
    return env;
}

Test(cd_more, my_chdir_not_a_directory)
{
    char template[] = "/tmp/ms_cd_file_XXXXXX";
    char cwd[4096] = {0};
    char *pwd = NULL;
    char **env = NULL;
    int fd = mkstemp(template);

    cr_assert_geq(fd, 0);
    close(fd);
    cr_assert_not_null(getcwd(cwd, sizeof(cwd)));
    pwd = my_strcat("PWD=", cwd);
    cr_assert_not_null(pwd);
    env = make_env_pwd(pwd);
    free(pwd);
    cr_assert_not_null(env);
    cr_assert_eq(my_chdir(template, &env), 84);
    unlink(template);
    free_array(env);
}

Test(cd_more, my_chdir_success_updates_env)
{
    char dir_template[] = "/tmp/ms_cd_dir_XXXXXX";
    char cwd[4096] = {0};
    char *pwd = NULL;
    char **env = NULL;
    char *new_pwd = NULL;
    char *old_pwd = NULL;
    char *dir = mkdtemp(dir_template);

    cr_assert_not_null(dir);
    cr_assert_not_null(getcwd(cwd, sizeof(cwd)));
    pwd = my_strcat("PWD=", cwd);
    cr_assert_not_null(pwd);
    env = make_env_pwd(pwd);
    free(pwd);
    cr_assert_not_null(env);
    cr_assert_eq(my_chdir(dir, &env), 0);
    new_pwd = my_getenv("PWD", env);
    old_pwd = my_getenv("OLDPWD", env);
    cr_assert_not_null(new_pwd);
    cr_assert_not_null(old_pwd);
    cr_assert_str_eq(new_pwd, dir);
    cr_assert_str_eq(old_pwd, cwd);
    chdir(cwd);
    rmdir(dir);
    free_array(env);
}

Test(exec_cmd_args_more, null_inputs)
{
    char *env[] = {"PATH=/bin:/usr/bin", NULL};

    cr_assert_eq(exec_cmd_args(NULL, env), 84);
    cr_assert_eq(exec_cmd_args_nofork(NULL, env), 84);
    cr_assert_eq(exec_cmd_args((char *[]) {NULL}, env), 84);
    cr_assert_eq(exec_cmd_args_nofork((char *[]) {NULL}, env), 84);
    cr_assert_eq(exec_cmd_args((char *[]) {"ls", NULL}, NULL), 84);
    cr_assert_eq(exec_cmd_args_nofork((char *[]) {"ls", NULL}, NULL), 84);
}

Test(exec_cmd_args_more, absolute_success)
{
    char *env[] = {"PATH=/bin:/usr/bin", NULL};
    char *args[] = {"/bin/true", NULL};

    cr_assert_eq(exec_cmd_args(args, env), 0);
}

Test(exec_cmd_args_more, absolute_missing_and_path_modes,
    .init = cr_redirect_stderr)
{
    char *env_ok[] = {"PATH=/bin:/usr/bin", NULL};
    char *args_abs[] = {"/no/such/cmd", NULL};
    char *args_rel[] = {"./no/such/cmd", NULL};

    cr_assert_eq(exec_cmd_args(args_abs, env_ok), 1);
    cr_assert_eq(exec_cmd_args(args_rel, env_ok), 1);
    cr_assert_eq(exec_cmd_args_nofork(args_abs, env_ok), 1);
    cr_assert_eq(exec_cmd_args_nofork(args_rel, env_ok), 1);
}

Test(exec_cmd_args_more, no_path_and_not_found_messages,
    .init = cr_redirect_stderr)
{
    char *args[] = {"not_a_real_cmd", NULL};
    char *env_no_path[] = {"HOME=/tmp", NULL};
    char *env_bad_path[] = {"PATH=/definitely_missing_1:/definitely_missing_2",
        NULL};

    cr_assert_eq(exec_cmd_args(args, env_no_path), 1);
    cr_assert_eq(exec_cmd_args(args, env_bad_path), 1);
    cr_assert_eq(exec_cmd_args_nofork(args, env_no_path), 84);
    cr_assert_eq(exec_cmd_args_nofork(args, env_bad_path), 0);
}
