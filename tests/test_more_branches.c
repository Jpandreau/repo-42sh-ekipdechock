/*
** EPITECH PROJECT, 2026
** test_more_branches
** File description:
** extra branch coverage without source injection
*/

#include <criterion/criterion.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include "base.h"
#include "buildin.h"
#include "my.h"
#include "small_headers.h"
#include "tree.h"

int unsetenv_buildin(char *arg, char ***env);

Test(env_manage_more, unsetenv_buildin_null_inputs)
{
    char **env = malloc(sizeof(char *) * 1);

    cr_assert_not_null(env);
    env[0] = NULL;
    cr_assert_eq(unsetenv_buildin(NULL, &env), 84);
    cr_assert_eq(unsetenv_buildin("A", NULL), 84);
    free(env);
}

Test(env_manage_more, unsetenv_buildin_empty_words)
{
    char **env = malloc(sizeof(char *) * 2);

    cr_assert_not_null(env);
    env[0] = my_strdup("A=1");
    env[1] = NULL;
    cr_assert_eq(unsetenv_buildin("    ", &env), 84);
    free_array(env);
}

Test(env_manage_more, unsetenv_buildin_remove_many)
{
    char **env = malloc(sizeof(char *) * 4);

    cr_assert_not_null(env);
    env[0] = my_strdup("A=1");
    env[1] = my_strdup("B=2");
    env[2] = my_strdup("C=3");
    env[3] = NULL;
    cr_assert_eq(unsetenv_buildin("A C", &env), 0);
    cr_assert_str_eq(env[0], "B=2");
    cr_assert_null(env[1]);
    free_array(env);
}

Test(env_manage_more, setenv_unsetenv_args_env_null)
{
    char *set_args[] = {"setenv", "A", "1", NULL};
    char *unset_args[] = {"unsetenv", "A", NULL};

    cr_assert_eq(setenv_buildin_args(set_args, NULL), 84);
    cr_assert_eq(unsetenv_buildin_args(unset_args, NULL), 84);
}

Test(cd_path_more, expand_tilde_relative_home)
{
    char cwd[4096] = {0};
    char rel_home[] = "ms_rel_home_dir";
    char *expected = NULL;
    char **env = malloc(sizeof(char *) * 2);
    char *res = NULL;

    cr_assert_not_null(env);
    cr_assert_not_null(getcwd(cwd, sizeof(cwd)));
    mkdir(rel_home, 0755);
    env[0] = my_strcat("HOME=", rel_home);
    env[1] = NULL;
    res = expand_tilde_path("~/docs", &env);
    expected = my_strcat(cwd, "/ms_rel_home_dir/docs");
    cr_assert_not_null(res);
    cr_assert_not_null(expected);
    cr_assert_str_eq(res, expected);
    free(expected);
    free(res);
    free_array(env);
    rmdir(rel_home);
}

Test(cd_path_more, chdir_home_relative_home)
{
    char cwd[4096] = {0};
    char rel_home[] = "ms_rel_home_run";
    char **env = malloc(sizeof(char *) * 3);

    cr_assert_not_null(env);
    cr_assert_not_null(getcwd(cwd, sizeof(cwd)));
    mkdir(rel_home, 0755);
    env[0] = my_strcat("HOME=", rel_home);
    env[1] = my_strcat("PWD=", cwd);
    env[2] = NULL;
    cr_assert_eq(chdir_home(&env), 0);
    chdir(cwd);
    free_array(env);
    rmdir(rel_home);
}

Test(cd_path_more, expand_tilde_null_arg)
{
    char *env_data[] = {"HOME=/tmp", NULL};
    char **env = env_data;

    cr_assert_null(expand_tilde_path(NULL, &env));
}

Test(cd_more, cd_dash_success)
{
    char cwd[4096] = {0};
    char tmp[] = "/tmp";
    char **env = malloc(sizeof(char *) * 4);
    char *args[] = {"cd", "-", NULL};

    cr_assert_not_null(env);
    cr_assert_not_null(getcwd(cwd, sizeof(cwd)));
    env[0] = my_strcat("PWD=", tmp);
    env[1] = my_strcat("OLDPWD=", cwd);
    env[2] = my_strdup("HOME=/tmp");
    env[3] = NULL;
    cr_assert_eq(cd_buildin_args(args, &env), 0);
    chdir(cwd);
    free_array(env);
}

Test(cd_more, my_chdir_permission_denied)
{
    char cwd[4096] = {0};
    char dir_template[] = "/tmp/ms_cd_perm_XXXXXX";
    char *dir = mkdtemp(dir_template);
    char **env = malloc(sizeof(char *) * 2);

    if (getuid() == 0) {
        rmdir(dir);
        free(env);
        cr_skip_test("running as root, chmod has no effect");
    }
    cr_assert_not_null(dir);
    cr_assert_not_null(env);
    cr_assert_not_null(getcwd(cwd, sizeof(cwd)));
    env[0] = my_strcat("PWD=", cwd);
    env[1] = NULL;
    chmod(dir, 0000);
    cr_assert_eq(my_chdir(dir, &env), 84);
    chmod(dir, 0700);
    rmdir(dir);
    free_array(env);
}

Test(main_helpers_more, handle_pipe_line_runs_tree)
{
    char **env = malloc(sizeof(char *) * 2);
    char line[] = "env\n";
    int exit_code = 0;

    cr_assert_not_null(env);
    env[0] = my_strdup("PATH=/bin:/usr/bin");
    env[1] = NULL;
    cr_assert_eq(handle_pipe_line(line, &env, &exit_code), 0);
    cr_assert_eq(exit_code, 0);
    free_array(env);
}

Test(main_helpers_more, handle_pipe_line_tree_null)
{
    char **env = malloc(sizeof(char *) * 2);
    char line[] = ";\n";
    int exit_code = 0;

    cr_assert_not_null(env);
    env[0] = my_strdup("PATH=/bin:/usr/bin");
    env[1] = NULL;
    cr_assert_eq(handle_pipe_line(line, &env, &exit_code), 0);
    free_array(env);
}

Test(main_helpers_more, run_line_exec_path_returns_0)
{
    char **env = malloc(sizeof(char *) * 2);
    int exit_code = 0;

    cr_assert_not_null(env);
    env[0] = my_strdup("PATH=/bin:/usr/bin");
    env[1] = NULL;
    cr_assert_eq(run_line("env", &env, &exit_code), 0);
    cr_assert_eq(exit_code, 0);
    free_array(env);
}

Test(script_helpers_more, init_exec_runs_tree)
{
    char **env = malloc(sizeof(char *) * 2);
    char *line = my_strdup("env");
    history_t history = {0};

    cr_assert_not_null(env);
    cr_assert_not_null(line);
    env[0] = my_strdup("PATH=/bin:/usr/bin");
    env[1] = NULL;
    history_init(&history);
    cr_assert_eq(init_exec(&line, &env, &history), 0);
    cr_assert_null(line);
    history_destroy(&history);
    free_array(env);
}

Test(exec_args_more, nofork_absolute_directory_execve_fails)
{
    char *args[] = {"/tmp", NULL};
    char *env[] = {"PATH=/bin:/usr/bin", NULL};

    cr_assert_eq(exec_cmd_args_nofork(args, env), 84);
}

Test(exec_args_more, fork_absolute_directory_execve_fails)
{
    char *args[] = {"/tmp", NULL};
    char *env[] = {"PATH=/bin:/usr/bin", NULL};

    cr_assert_eq(exec_cmd_args(args, env), 1);
}

Test(exec_args_more, nofork_found_true_with_directory_execve_fail)
{
    char dir_template[] = "/tmp/ms_nofork_dir_XXXXXX";
    char *dir = mkdtemp(dir_template);
    char *slash = strrchr(dir, '/');
    char *name = slash + 1;
    char *path_env = my_strcat("PATH=/tmp", "");
    char *env[] = {path_env, NULL};
    char *args[] = {name, NULL};

    cr_assert_not_null(dir);
    cr_assert_not_null(path_env);
    cr_assert_eq(exec_cmd_args_nofork(args, env), 0);
    rmdir(dir);
    free(path_env);
}

Test(exit_in_pipe_more, null_code_and_invalid_pointers)
{
    tree_t *pipe_node = new_node(TOKEN_PIPE);

    pipe_node->left = new_node(TOKEN_CMD);
    pipe_node->left->args = malloc(sizeof(char *) * 2);
    pipe_node->left->args[0] = my_strdup("env");
    pipe_node->left->args[1] = NULL;
    pipe_node->right = new_node(TOKEN_CMD);
    pipe_node->right->args = malloc(sizeof(char *) * 2);
    pipe_node->right->args[0] = my_strdup("env");
    pipe_node->right->args[1] = NULL;
    cr_assert_eq(find_exit_in_pipe(pipe_node, NULL, NULL), 0);
    free_tree(pipe_node);
}

Test(exit_in_pipe_more, exit_without_arg_in_pipe)
{
    tree_t *pipe_node = new_node(TOKEN_PIPE);
    int code = 99;
    int invalid = 99;

    pipe_node->left = new_node(TOKEN_CMD);
    pipe_node->left->args = malloc(sizeof(char *) * 2);
    pipe_node->left->args[0] = my_strdup("exit");
    pipe_node->left->args[1] = NULL;
    pipe_node->right = new_node(TOKEN_CMD);
    pipe_node->right->args = malloc(sizeof(char *) * 2);
    pipe_node->right->args[0] = my_strdup("env");
    pipe_node->right->args[1] = NULL;
    cr_assert_eq(find_exit_in_pipe(pipe_node, &code, &invalid), 1);
    cr_assert_eq(code, 0);
    cr_assert_eq(invalid, 0);
    free_tree(pipe_node);
}

Test(exit_in_pipe_more, exit_too_many_args_marks_invalid)
{
    tree_t *pipe_node = new_node(TOKEN_PIPE);
    int code = 0;
    int invalid = 0;

    pipe_node->left = new_node(TOKEN_CMD);
    pipe_node->left->args = malloc(sizeof(char *) * 4);
    pipe_node->left->args[0] = my_strdup("exit");
    pipe_node->left->args[1] = my_strdup("1");
    pipe_node->left->args[2] = my_strdup("2");
    pipe_node->left->args[3] = NULL;
    pipe_node->right = new_node(TOKEN_CMD);
    pipe_node->right->args = malloc(sizeof(char *) * 2);
    pipe_node->right->args[0] = my_strdup("env");
    pipe_node->right->args[1] = NULL;
    cr_assert_eq(find_exit_in_pipe(pipe_node, &code, &invalid), 0);
    cr_assert_eq(invalid, 1);
    free_tree(pipe_node);
}

Test(exit_in_pipe_more, exit_invalid_arg_marks_invalid)
{
    tree_t *pipe_node = new_node(TOKEN_PIPE);
    int code = 0;
    int invalid = 0;

    pipe_node->left = new_node(TOKEN_CMD);
    pipe_node->left->args = malloc(sizeof(char *) * 3);
    pipe_node->left->args[0] = my_strdup("exit");
    pipe_node->left->args[1] = my_strdup("abc");
    pipe_node->left->args[2] = NULL;
    pipe_node->right = new_node(TOKEN_CMD);
    pipe_node->right->args = malloc(sizeof(char *) * 2);
    pipe_node->right->args[0] = my_strdup("env");
    pipe_node->right->args[1] = NULL;
    cr_assert_eq(find_exit_in_pipe(pipe_node, &code, &invalid), 0);
    cr_assert_eq(invalid, 1);
    free_tree(pipe_node);
}
