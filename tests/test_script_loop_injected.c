/*
** EPITECH PROJECT, 2026
** test_script_loop_injected
** File description:
** injected tests for hard-to-reach script_loop branches
*/

#include <criterion/criterion.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include "base.h"
#include "tree.h"
#include "small_headers.h"

static int g_isatty = 0;
static int g_write_calls = 0;
static char g_last_write[32] = {0};
static int g_handle_line_status = 0;
static int g_getline_idx = 0;
static ssize_t g_getline_ret[4] = {0};
static const char *g_getline_txt[4] = {0};

int mocked_isatty(int fd)
{
    (void)fd;
    return g_isatty;
}

ssize_t mocked_getline(char **line, size_t *len, FILE *stream)
{
    ssize_t ret = g_getline_ret[g_getline_idx];
    const char *txt = g_getline_txt[g_getline_idx];

    (void)stream;
    g_getline_idx++;
    if (ret <= 0)
        return ret;
    *line = malloc((size_t)ret + 1);
    if (*line == NULL)
        return -1;
    memcpy(*line, txt, (size_t)ret);
    (*line)[ret] = '\0';
    *len = (size_t)ret + 1;
    return ret;
}

ssize_t mocked_write(int fd, const void *buf, size_t count)
{
    size_t copy = count;

    (void)fd;
    g_write_calls++;
    if (copy > sizeof(g_last_write) - 1)
        copy = sizeof(g_last_write) - 1;
    memcpy(g_last_write, buf, copy);
    g_last_write[copy] = '\0';
    return (ssize_t)count;
}

void mocked_handle_sigint_stub(int sig)
{
    (void)sig;
}

void (*mocked_signal(int signum, void (*handler)(int)))(int)
{
    (void)signum;
    return handler;
}

int mocked_handle_line(char **line, char ***env, exec_ctx_t *ctx)
{
    (void)env;
    (void)ctx;
    free(*line);
    *line = NULL;
    return g_handle_line_status;
}

int mocked_history_init(history_t *history)
{
    (void)history;
    return 0;
}

void mocked_history_destroy(history_t *history)
{
    (void)history;
}

int mocked_history_add(history_t *history, char *line)
{
    (void)history;
    (void)line;
    return 0;
}

int mocked_history_expand_line(history_t *history, char *line, char **expanded)
{
    (void)history;
    *expanded = my_strdup(line);
    return *expanded == NULL ? 84 : 0;
}

int mocked_interactive_getline(char **line, size_t *len, history_t *history)
{
    ssize_t ret = g_getline_ret[g_getline_idx];
    const char *txt = g_getline_txt[g_getline_idx];

    (void)history;
    g_getline_idx++;
    if (ret == -1)
        return -1;
    if (ret == 1) {
        *line = NULL;
        return 1;
    }
    *line = malloc((size_t)ret + 1);
    if (*line == NULL)
        return 84;
    memcpy(*line, txt, (size_t)ret);
    (*line)[ret] = '\0';
    if (len != NULL)
        *len = (size_t)ret;
    return 0;
}

#define final_exit_script_loop tested_final_exit_script_loop
#define clean_getline tested_clean_getline
#define script_loop tested_script_loop
#define isatty mocked_isatty
#define getline mocked_getline
#define write mocked_write
#define signal mocked_signal
#define handle_sigint mocked_handle_sigint_stub
#define handle_line mocked_handle_line
#define history_init mocked_history_init
#define history_destroy mocked_history_destroy
#define history_add mocked_history_add
#define history_expand_line mocked_history_expand_line
#define interactive_get_line mocked_interactive_getline
#include "../src/script_loop.c"
#undef interactive_getline
#undef history_expand_line
#undef history_add
#undef history_destroy
#undef history_init
#undef handle_line
#undef handle_sigint
#undef signal
#undef write
#undef getline
#undef isatty
#undef script_loop
#undef clean_getline
#undef final_exit_script_loop

static void reset_mocks(void)
{
    g_isatty = 0;
    g_write_calls = 0;
    g_last_write[0] = '\0';
    g_handle_line_status = 0;
    g_getline_idx = 0;
    for (int i = 0; i < 4; i++) {
        g_getline_ret[i] = 0;
        g_getline_txt[i] = NULL;
    }
}

Test(script_loop_injected, clean_getline_eof_tty_prints_exit)
{
    char *line = NULL;
    size_t len = 0;

    reset_mocks();
    g_isatty = 1;
    g_getline_ret[0] = -1;
    cr_assert_eq(tested_clean_getline(&line, &len), -1);
    cr_assert_gt(g_write_calls, 0);
    cr_assert_str_eq(g_last_write, "exit\n");
}

Test(script_loop_injected, script_loop_handles_exit_status_and_prints)
{
    char **env = malloc(sizeof(char *) * 2);

    cr_assert_not_null(env);
    env[0] = my_strdup("PATH=/bin");
    env[1] = NULL;
    reset_mocks();
    g_isatty = 1;
    g_getline_ret[0] = 2;
    g_getline_txt[0] = "x\n";
    g_handle_line_status = make_exit_status(7);
    cr_assert_eq(tested_script_loop(env), 7);
    cr_assert_gt(g_write_calls, 0);
    cr_assert_str_eq(g_last_write, "exit\n");
}

Test(script_loop_injected, script_loop_breaks_on_84)
{
    char **env = malloc(sizeof(char *) * 2);

    cr_assert_not_null(env);
    env[0] = my_strdup("PATH=/bin");
    env[1] = NULL;
    reset_mocks();
    g_isatty = 0;
    g_getline_ret[0] = -1;
    cr_assert_eq(tested_script_loop(env), 84);
}

Test(script_loop_injected, script_loop_null_env)
{
    reset_mocks();
    cr_assert_eq(tested_script_loop(NULL), 84);
}

Test(script_loop_injected, script_loop_status_zero_then_eof)
{
    char **env = malloc(sizeof(char *) * 2);

    cr_assert_not_null(env);
    env[0] = my_strdup("PATH=/bin");
    env[1] = NULL;
    reset_mocks();
    g_isatty = 0;
    g_getline_ret[0] = 2;
    g_getline_txt[0] = "z\n";
    g_getline_ret[1] = -1;
    g_handle_line_status = 0;
    cr_assert_eq(tested_script_loop(env), 84);
}

Test(script_loop_injected, clean_getline_returns_1_on_empty_line)
{
    char *line = NULL;
    size_t len = 0;

    reset_mocks();
    g_getline_ret[0] = 1;
    g_getline_txt[0] = "\n";
    cr_assert_eq(tested_clean_getline(&line, &len), 1);
    free(line);
}
