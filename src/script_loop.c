/*
** EPITECH PROJECT, 2026
** script_loop
** File description:
** script_loop
*/

#include "my.h"
#include "base.h"
#include "tree.h"
#include "small_headers.h"
#include "job_control.h"
#include "shell.h"

static int expand_sudo_history(char *line, history_t *history, char **expanded)
{
    char *cmd = NULL;
    char *prefix = NULL;
    int idx = 5;
    int status = 0;

    if (my_strncmp(line, "sudo ", 5) != 0)
        return -1;
    while (line[idx] == ' ' || line[idx] == '\t')
        idx++;
    status = history_expand_line(history, line + idx, &cmd);
    if (status != 0 || cmd == NULL)
        return status;
    prefix = my_strndup(line, idx);
    *expanded = prefix == NULL ? NULL : my_strcat(prefix, cmd);
    free(prefix);
    free(cmd);
    return *expanded == NULL ? 84 : 0;
}

static int expand_and_store_history(char **line, history_t *history)
{
    char *expanded = NULL;
    int status = expand_sudo_history(*line, history, &expanded);

    if (status == -1 && history != NULL && history->size > 0)
        status = history_expand_line(history, *line, &expanded);
    if (status == -1)
        expanded = my_strdup(*line);
    free(*line);
    *line = NULL;
    if (status == 84 || expanded == NULL)
        return 84;
    *line = expanded;
    if (history_add(history, *line) == 84) {
        free(*line);
        *line = NULL;
        return 84;
    }
    return 0;
}

static int handle_interactive_line(char **line, char ***env, exec_ctx_t *ctx)
{
    int status = expand_and_store_history(line, ctx->history);

    if (status != 0)
        return status;
    if (*line == NULL)
        return 0;
    status = handle_line(line, env, ctx);
    return status;
}

static int read_next_line(char **line, size_t *len, history_t *history)
{
    if (isatty(STDIN_FILENO))
        return interactive_getline(line, len, history);
    return clean_getline(line, len);
}

static int handle_noninteractive_line(char **line, char ***env, size_t *len,
    exec_ctx_t *ctx)
{
    int status = handle_line(line, env, ctx);

    if (is_exit_status(status))
        return status;
    *len = 0;
    return 0;
}

static int loop_step(char **line, size_t *len, char ***env, exec_ctx_t *ctx)
{
    int status = 0;

    job_control_reap(ctx->job);
    status = read_next_line(line, len, ctx->history);
    if (status == 1) {
        free(*line);
        *line = NULL;
        return 0;
    }
    if (status == -1 || *line == NULL)
        return 84;
    if (isatty(STDIN_FILENO)) {
        status = handle_interactive_line(line, env, ctx);
        if (is_exit_status(status))
            return status;
        return status;
    }
    return handle_noninteractive_line(line, env, len, ctx);
}

static int handle_exit_status(int *exit_code)
{
    if (!is_exit_status(*exit_code))
        return 0;
    *exit_code = exit_status_code(*exit_code);
    if (isatty(STDIN_FILENO))
        write(1, "exit\n", 5);
    return 1;
}

int final_exit_script_loop(int exit_code, char *line, char **env)
{
    free(line);
    free_array(env);
    return exit_code;
}

int clean_getline(char **line, size_t *len)
{
    ssize_t size = 0;

    size = getline(line, len, stdin);
    if (size == -1) {
        if (isatty(STDIN_FILENO))
            write(1, "exit\n", 5);
        return -1;
    }
    if (size == 1)
        return 1;
    (*line)[size - 1] = '\0';
    return 0;
}

int script_loop(char **env)
{
    int exit_code = 0;
    char *line = NULL;
    size_t len = 0;
    history_t history = {0};
    job_state_t job = {0};
    shell_t shell = {NULL, NULL};
    exec_ctx_t ctx = {&history, &job, &shell};

    if (env == NULL || init_shell_ctx(&history, &shell) == 84)
        return 84;
    while (exit_code != 84) {
        exit_code = loop_step(&line, &len, &env, &ctx);
        if (handle_exit_status(&exit_code))
            break;
    }
    history_destroy(&history);
    shell_destroy(&shell);
    return final_exit_script_loop(exit_code, line, env);
}
