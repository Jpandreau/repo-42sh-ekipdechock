/*
** EPITECH PROJECT, 2026
** main
** File description:
** main
*/

#include "my.h"
#include "base.h"
#include "tree.h"
#include "small_headers.h"
#include "buildin.h"
#include "job_control.h"

static int pipe_input_fail(char **env)
{
    free_array(env);
    return 84;
}

static char **add_default_nlspath(char **env)
{
    char *nlspath =
        "NLSPATH=/usr/share/locale/%L/LC_MESSAGES/%N.cat:/usr/share/"
        "locale/%l/LC_MESSAGES/%N.cat";
    char *value = NULL;

    if (my_getenv("NLSPATH", env) != NULL)
        return env;
    value = my_strdup(nlspath);
    if (value == NULL)
        return env;
    return my_array_add(env, value);
}

static void pipe_cleanup(char *line, char **env, history_t *hist,
    shell_t *shell)
{
    history_destroy(hist);
    shell_destroy(shell);
    free(line);
    free_array(env);
}

static int pipe_run_loop(char **line, size_t *len, char ***env,
    exec_ctx_t *ctx)
{
    int status = 0;

    while (clean_getline(line, len) != -1) {
        status = handle_line(line, env, ctx);
        if (is_exit_status(status))
            return exit_status_code(status);
    }
    return 0;
}

int exec_all_cmd_file(char *content, char ***env)
{
    int exit_code = 0;
    char *input_line = NULL;
    history_t history = {0};
    job_state_t job = {0};
    shell_t shell = {NULL, NULL};
    exec_ctx_t ctx = {&history, &job, &shell};

    if (history_init(&history) == 84 || shell_init(&shell) == 84)
        return 84;
    for (input_line = strtok(content, "\n"); input_line != NULL;
        input_line = strtok(NULL, "\n")) {
        if (input_line[0] == '\n')
            continue;
        if (run_line(input_line, env, &exit_code, &ctx))
            break;
    }
    history_destroy(&history);
    shell_destroy(&shell);
    return exit_code;
}

int command_file(char *filename, char **env)
{
    int exit_code = 0;
    int file = 0;
    char *content = NULL;
    struct stat st;

    file = open_and_stat(filename, &st);
    if (file == -1)
        return 84;
    if (read_content(file, &st, &content) != 0)
        return 84;
    exit_code = exec_all_cmd_file(content, &env);
    free(content);
    free_array(env);
    return exit_code;
}

int pipe_input(char **env)
{
    char *line = NULL;
    size_t len = 0;
    int exit_code = 0;
    history_t history = {0};
    job_state_t job = {0};
    shell_t shell = {NULL, NULL};
    exec_ctx_t ctx = {&history, &job, &shell};

    if (history_init(&history) == 84 || shell_init(&shell) == 84)
        return pipe_input_fail(env);
    exit_code = pipe_run_loop(&line, &len, &env, &ctx);
    pipe_cleanup(line, env, &history, &shell);
    return exit_code;
}

int main(int ac, char **av, char **env)
{
    char **my_env = NULL;

    if (env == NULL)
        return 84;
    signal(SIGINT, handle_sigint);
    job_control_init();
    my_env = my_array_dup(env);
    if (my_env == NULL)
        return 84;
    my_env = add_default_nlspath(my_env);
    if (my_env == NULL)
        return 84;
    if (ac > 1)
        return command_file(av[1], my_env);
    if (!isatty(STDIN_FILENO))
        return pipe_input(my_env);
    return script_loop(my_env);
}
