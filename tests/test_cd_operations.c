/*
** EPITECH PROJECT, 2026
** test_cd_operations
** File description:
** Tests pour le builtin cd et les opérations de gestion de chemins
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "base.h"
#include "buildin.h"
#include "my.h"

Test(expand_tilde_path, tilde_alone)
{
    char *env[] = {"HOME=/home/user", NULL};
    char **env_ptr = env;
    char *result = expand_tilde_path("~", &env_ptr);
    cr_assert_not_null(result);
    cr_assert_str_eq(result, "/home/user");
    free(result);
}

Test(expand_tilde_path, tilde_with_path)
{
    char *env[] = {"HOME=/home/user", NULL};
    char **env_ptr = env;
    char *result = expand_tilde_path("~/documents", &env_ptr);
    cr_assert_not_null(result);
    cr_assert_str_eq(result, "/home/user/documents");
    free(result);
}

Test(expand_tilde_path, no_tilde)
{
    char *env[] = {"HOME=/home/user", NULL};
    char **env_ptr = env;
    char *result = expand_tilde_path("/tmp", &env_ptr);
    cr_assert_not_null(result);
    cr_assert_str_eq(result, "/tmp");
    free(result);
}

Test(expand_tilde_path, tilde_not_at_start)
{
    char *env[] = {"HOME=/home/user", NULL};
    char **env_ptr = env;
    char *result = expand_tilde_path("~user", &env_ptr);
    cr_assert_not_null(result);
    cr_assert_str_eq(result, "~user");
    free(result);
}

Test(expand_tilde_path, relative_path)
{
    char *env[] = {"HOME=/home/user", NULL};
    char **env_ptr = env;
    char *result = expand_tilde_path("./test", &env_ptr);
    cr_assert_not_null(result);
    cr_assert_str_eq(result, "./test");
    free(result);
}

Test(expand_tilde_path, tilde_with_deep_path)
{
    char *env[] = {"HOME=/home/user", NULL};
    char **env_ptr = env;
    char *result = expand_tilde_path("~/docs/work/project", &env_ptr);
    cr_assert_not_null(result);
    cr_assert_str_eq(result, "/home/user/docs/work/project");
    free(result);
}

Test(expand_tilde_path, no_home_env)
{
    char *env[] = {NULL};
    char **env_ptr = env;
    char *result = expand_tilde_path("~", &env_ptr);
    cr_assert_null(result);
}

Test(expand_tilde_path, empty_home)
{
    char *env[] = {"HOME=", NULL};
    char **env_ptr = env;
    char *result = expand_tilde_path("~", &env_ptr);
    cr_assert_null(result);
}
