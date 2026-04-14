/*
** EPITECH PROJECT, 2026
** test_main_and_script_helpers
** File description:
** coverage for main_helpers and script_loop helpers
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdio.h>
#include "my.h"
#include "base.h"
#include "small_headers.h"
#include "tree.h"

static char **make_heap_env_one(char *value)
{
    char **env = malloc(sizeof(char *) * 2);

    if (env == NULL)
        return NULL;
    env[0] = my_strdup(value);
    env[1] = NULL;
    return env;
}

Test(main_helpers, open_and_stat_missing_file)
{
    struct stat st;

    cr_assert_eq(open_and_stat("/no/such/file/for_open_and_stat", &st), -1);
}

Test(main_helpers, open_and_stat_and_read_content_ok)
{
    char template[] = "/tmp/ms_read_content_XXXXXX";
    int fd = mkstemp(template);
    struct stat st;
    int file = 0;
    char *content = NULL;

    cr_assert_geq(fd, 0);
    cr_assert_eq(write(fd, "abc", 3), 3);
    close(fd);
    file = open_and_stat(template, &st);
    cr_assert_geq(file, 0);
    cr_assert_eq(read_content(file, &st, &content), 0);
    cr_assert_not_null(content);
    cr_assert_str_eq(content, "abc");
    free(content);
    unlink(template);
}

Test(main_helpers, run_line_exit_builtin)
{
    char **env = make_heap_env_one("PATH=/bin:/usr/bin");
    int exit_code = 0;

    cr_assert_not_null(env);
    cr_assert_eq(run_line("exit 7", &env, &exit_code), 1);
    cr_assert_eq(exit_code, 7);
    free_array(env);
}

Test(main_helpers, run_line_invalid_tree_returns_0)
{
    char **env = make_heap_env_one("PATH=/bin:/usr/bin");
    int exit_code = 0;

    cr_assert_not_null(env);
    cr_assert_eq(run_line(";", &env, &exit_code), 0);
    cr_assert_eq(exit_code, 0);
    free_array(env);
}

Test(main_helpers, run_line_error_84_becomes_stop)
{
    char **env = make_heap_env_one("PATH=/bin:/usr/bin");
    int exit_code = 0;

    cr_assert_not_null(env);
    cr_assert_eq(run_line("unsetenv", &env, &exit_code), 1);
    cr_assert_eq(exit_code, 0);
    free_array(env);
}

Test(main_helpers, handle_pipe_line_empty_line)
{
    char **env = make_heap_env_one("PATH=/bin:/usr/bin");
    int exit_code = 0;
    char input[] = "\n";

    cr_assert_not_null(env);
    cr_assert_eq(handle_pipe_line(input, &env, &exit_code), 0);
    cr_assert_eq(exit_code, 0);
    free_array(env);
}

Test(main_helpers, handle_pipe_line_exit)
{
    char **env = make_heap_env_one("PATH=/bin:/usr/bin");
    int exit_code = 0;
    char input[] = "exit 3\n";

    cr_assert_not_null(env);
    cr_assert_eq(handle_pipe_line(input, &env, &exit_code), 1);
    cr_assert_eq(exit_code, 3);
    free_array(env);
}

Test(script_loop_helpers, init_exec_null_tree_frees_line)
{
    char **env = make_heap_env_one("PATH=/bin:/usr/bin");
    char *line = my_strdup(";");

    cr_assert_not_null(env);
    cr_assert_not_null(line);
    cr_assert_eq(init_exec(&line, &env), 0);
    cr_assert_null(line);
    free_array(env);
}

Test(script_loop_helpers, handle_line_exit_status)
{
    char **env = make_heap_env_one("PATH=/bin:/usr/bin");
    char *line = my_strdup("exit 9");

    cr_assert_not_null(env);
    cr_assert_not_null(line);
    cr_assert_eq(handle_line(&line, &env), make_exit_status(9));
    cr_assert_null(line);
    free_array(env);
}

Test(script_loop_helpers, handle_line_exit_syntax_returns_0)
{
    char **env = make_heap_env_one("PATH=/bin:/usr/bin");
    char *line = my_strdup("exit a");

    cr_assert_not_null(env);
    cr_assert_not_null(line);
    cr_assert_eq(handle_line(&line, &env), 0);
    cr_assert_null(line);
    free_array(env);
}

Test(script_loop, clean_getline_regular_line)
{
    int fd_pipe[2] = {0};
    int saved_stdin = dup(STDIN_FILENO);
    char *line = NULL;
    size_t len = 0;

    cr_assert_eq(pipe(fd_pipe), 0);
    write(fd_pipe[1], "abc\n", 4);
    close(fd_pipe[1]);
    dup2(fd_pipe[0], STDIN_FILENO);
    close(fd_pipe[0]);
    cr_assert_eq(clean_getline(&line, &len), 0);
    cr_assert_str_eq(line, "abc");
    free(line);
    dup2(saved_stdin, STDIN_FILENO);
    close(saved_stdin);
}

Test(script_loop, clean_getline_empty_line)
{
    int fd_pipe[2] = {0};
    int saved_stdin = dup(STDIN_FILENO);
    char *line = NULL;
    size_t len = 0;

    cr_assert_eq(pipe(fd_pipe), 0);
    write(fd_pipe[1], "\n", 1);
    close(fd_pipe[1]);
    dup2(fd_pipe[0], STDIN_FILENO);
    close(fd_pipe[0]);
    cr_assert_eq(clean_getline(&line, &len), 1);
    free(line);
    dup2(saved_stdin, STDIN_FILENO);
    close(saved_stdin);
}

Test(script_loop, clean_getline_eof)
{
    int fd_pipe[2] = {0};
    int saved_stdin = dup(STDIN_FILENO);
    char *line = NULL;
    size_t len = 0;

    cr_assert_eq(pipe(fd_pipe), 0);
    close(fd_pipe[1]);
    dup2(fd_pipe[0], STDIN_FILENO);
    close(fd_pipe[0]);
    cr_assert_eq(clean_getline(&line, &len), -1);
    free(line);
    dup2(saved_stdin, STDIN_FILENO);
    close(saved_stdin);
}

Test(script_loop, script_loop_null_env)
{
    cr_assert_eq(script_loop(NULL), 84);
}

Test(script_loop, script_loop_exit_code_from_pipe)
{
    int fd_pipe[2] = {0};
    int saved_stdin = dup(STDIN_FILENO);
    char **env = make_heap_env_one("PATH=/bin:/usr/bin");

    cr_assert_not_null(env);
    cr_assert_eq(pipe(fd_pipe), 0);
    write(fd_pipe[1], "exit 5\n", 7);
    close(fd_pipe[1]);
    dup2(fd_pipe[0], STDIN_FILENO);
    close(fd_pipe[0]);
    cr_assert_eq(script_loop(env), 5);
    dup2(saved_stdin, STDIN_FILENO);
    close(saved_stdin);
}

Test(script_loop, script_loop_handles_empty_line_then_exit)
{
    int fd_pipe[2] = {0};
    int saved_stdin = dup(STDIN_FILENO);
    char **env = make_heap_env_one("PATH=/bin:/usr/bin");

    cr_assert_not_null(env);
    cr_assert_eq(pipe(fd_pipe), 0);
    write(fd_pipe[1], "\nexit 0\n", 8);
    close(fd_pipe[1]);
    dup2(fd_pipe[0], STDIN_FILENO);
    close(fd_pipe[0]);
    cr_assert_eq(script_loop(env), 0);
    dup2(saved_stdin, STDIN_FILENO);
    close(saved_stdin);
}
