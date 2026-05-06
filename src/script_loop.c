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
#include "line_edition.h"

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
    char *old_cwd = getcwd(NULL, 0);

    if (status != 0) {
        free(old_cwd);
        return status;
    }
    if (*line == NULL) {
        free(old_cwd);
        return 0;
    }
    line_editor_suspend();
    status = handle_line(line, env, ctx);
    line_editor_resume();
    check_and_run_cwdcmd(old_cwd, env, ctx);
    free(old_cwd);
    return status;
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

static int check_ignoreof(exec_ctx_t *ctx)
{
    if (!isatty(STDIN_FILENO) || ctx == NULL || ctx->shell == NULL)
        return 84;
    if (get_local(ctx->shell->locals, "ignoreof") == NULL)
        return 84;
    write(STDOUT_FILENO, "Use 'exit' to exit.\n", 20);
    return 0;
}

static int loop_step(char **line, size_t *len, char ***env, exec_ctx_t *ctx)
{
    int status = 0;

    job_control_reap(ctx->job);
    if (isatty(STDIN_FILENO)) {
        run_hook("precmd", env, ctx);
        status = interactive_getline(line, len, ctx->history);
    } else
        status = clean_getline(line, len);
    if (status == 1) {
        free(*line);
        *line = NULL;
        return 0;
    }
    if (status == -1)
        return check_ignoreof(ctx);
    if (*line == NULL)
        return 84;
    if (isatty(STDIN_FILENO))
        return handle_interactive_line(line, env, ctx);
    return handle_noninteractive_line(line, env, len, ctx);
}

int final_exit_script_loop(int exit_code, char *line, char **env)
{
    free(line);
    free_array(env);
    return exit_code;
}

static int run_loop(char ***env, exec_ctx_t *ctx)
{
    int exit_code = 0;
    char *line = NULL;
    size_t len = 0;

    while (exit_code != 84) {
        exit_code = loop_step(&line, &len, env, ctx);
        if (!is_exit_status(exit_code))
            continue;
        exit_code = exit_status_code(exit_code);
        if (isatty(STDIN_FILENO))
            write(1, "exit\n", 5);
        break;
    }
    return final_exit_script_loop(exit_code, line, *env);
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
    history_t history = {0};
    job_state_t job = {0};
    shell_t shell = {NULL, NULL};
    exec_ctx_t ctx = {&history, &job, &shell};
    int ret = 0;

    if (env == NULL || init_shell_ctx(&history, &shell) == 84)
        return 84;
    if (isatty(STDIN_FILENO))
        line_editor_init();
    ret = run_loop(&env, &ctx);
    if (isatty(STDIN_FILENO))
        line_editor_stop();
    history_destroy(&history);
    shell_destroy(&shell);
    return ret;
}
